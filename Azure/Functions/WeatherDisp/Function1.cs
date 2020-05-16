using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using PuppeteerSharp;
using Microsoft.Extensions.Hosting;
using ImageMagick;
using Azure.WebJobs.Extensions.HttpApi;
using System.ComponentModel.DataAnnotations;

namespace WeatherDisp
{
    public class WeatherInfo : HttpFunctionBase
    {
        public WeatherInfo(IHttpContextAccessor httpContextAccessor) : base(httpContextAccessor)
        {
        }

        [FunctionName("WeatherInfo")]
        public async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", "post", Route = null)] DarkSkyRequestModel model,
            ILogger log, Microsoft.Azure.WebJobs.ExecutionContext context)
        {
            log.LogInformation("WeatherInfo triggerd");

            if (model == null || !TryValidateModel(model))
            {
                return BadRequest(ModelState);
            }
            
            if(context == null)
            {
                return BadRequest();
            }

            using (var browser = await Puppeteer.LaunchAsync(new LaunchOptions()
            {
                Headless = true,
                LogProcess = true,
                Args = new[]
                {
                    "--no-sandbox",
                    "--disable-web-security"
                }
            }))
            {
                using var page = await browser.NewPageAsync();
                await page.SetViewportAsync(new ViewPortOptions()
                {
                    Width = 298,
                    Height = 128
                });

                using (var fs = new StreamReader(Path.Combine(context.FunctionAppDirectory, Path.Combine("dist", "index.html"))))
                {
                    var html = (await fs.ReadToEndAsync())
                        .Replace("SET_YOUR_DARKSKY_KEY", model.DarkskyKey, StringComparison.OrdinalIgnoreCase)
                        .Replace("SET_YOUR_LAT", model.Lat, StringComparison.OrdinalIgnoreCase)
                        .Replace("SET_YOUR_LAG", model.Lag, StringComparison.OrdinalIgnoreCase);
                    await page.SetContentAsync(html, new NavigationOptions()
                    {
                        WaitUntil = new[] { WaitUntilNavigation.Networkidle0 }
                    });
                }

                using var im = new MagickImage(await page.ScreenshotDataAsync(
                        new ScreenshotOptions()
                        {
                            Type = ScreenshotType.Png
                        }));
                im.Map(new[] { 
                    new MagickColor(0, 0, 0), 
                    new MagickColor(255, 255, 255) 
                }, new QuantizeSettings() {
                    DitherMethod = DitherMethod.No
                });
                im.Quality = 100;
                return File(im.ToByteArray(MagickFormat.Jpg), "image/jpeg");
            }
        }
    }

    public class DarkSkyRequestModel
    {
        [Required]
        public string DarkskyKey { get; set; }

        [Required]
        public string Lat { get; set; }

        [Required]
        public string Lag { get; set; }
    }
}
