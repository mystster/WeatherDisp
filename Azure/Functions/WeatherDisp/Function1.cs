using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using Azure.WebJobs.Extensions.HttpApi;
using System.ComponentModel.DataAnnotations;
using PuppeteerSharp;
using ImageMagick;

namespace WeatherDisp
{
    public class WeatherInfo : HttpFunctionBase
    {
        static int execCounter;
        public WeatherInfo(IHttpContextAccessor httpContextAccessor) : base(httpContextAccessor)
        {
        }

        [FunctionName("WeatherInfo")]
        public async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", "post", Route = null)] DarkSkyRequestModel model,
            ILogger log, Microsoft.Azure.WebJobs.ExecutionContext context)
        {
            execCounter++;
            log.LogInformation($"WeatherInfo triggerd, count:{execCounter}");

            if (model == null || context == null)
            {
                return BadRequest();
            }

            var slotName = Environment.GetEnvironmentVariable("WEBSITE_SLOT_NAME", EnvironmentVariableTarget.Process);
            log.LogInformation($"SlotName:{slotName}");

            if (slotName != "Production")
            {
                // Slotの設定がある場合は、PR用と想定し、環境変数から情報を収集する
                if (int.TryParse(Environment.GetEnvironmentVariable("MAX_EXEC", EnvironmentVariableTarget.Process), out int maxCount)
                    && maxCount > 0
                    && execCounter <= maxCount)
                {
                    model.DarkskyKey = Environment.GetEnvironmentVariable("DARKSKY_API", EnvironmentVariableTarget.Process) ?? model.DarkskyKey;
                    model.Lag = Environment.GetEnvironmentVariable("LAG", EnvironmentVariableTarget.Process) ?? model.Lag;
                    model.Lat = Environment.GetEnvironmentVariable("LAT", EnvironmentVariableTarget.Process) ?? model.Lat;
                }
            }

            if (!TryValidateModel(model))
            {
                return BadRequest(ModelState);
            }

            log.LogInformation($"Lag:{model.Lag}, Lat:{model.Lat}");

            using var browser = await Puppeteer.LaunchAsync(new LaunchOptions()
            {
                Headless = true,
                LogProcess = true,
                Args = new[]
                {
                    "--no-sandbox",
                    "--disable-web-security"
                },
                DumpIO = true
            });
            using var page = await browser.NewPageAsync();
            page.Console += (target, e) => {
                switch (e.Message.Type)
                {
                    case ConsoleType.Error:
                        log.LogError($"ConsoleError:{e.Message.Text}");
                        break;
                    case ConsoleType.Warning:
                        log.LogWarning($"ConsoleWarning:{e.Message.Text}");
                        break;
                    default:
                        log.LogInformation($"Console{e.Message.Type}:{e.Message.Text}");
                        break;
                }
            };
            page.Error += (target, e) =>
            {
                log.LogError($"Error:{e.Error}");
            };
            page.PageError += (target, e) =>
            {
                log.LogError($"PageError:{e.Message}");
            };
            page.Response += (target, e) =>
            {
                log.LogInformation($"Response:{e.Response.Status}:{e.Response.StatusText}");
            };
            page.RequestFailed += (target, e) =>
            {
                log.LogError($"RequestFailed:{e.Request.Failure}");
            };
            await page.SetViewportAsync(new ViewPortOptions()
            {
                Width = 298,
                Height = 128
            });

            using (var fs = new StreamReader(Path.Combine(context.FunctionAppDirectory, Path.Combine("dist", "index.html"))))
            {
                var html = (await fs.ReadToEndAsync())
                    .Replace("DARKSKY_KEY_PLACEHOLDER", model.DarkskyKey, StringComparison.OrdinalIgnoreCase)
                    .Replace("LAT_PLACEHOLDER", model.Lat, StringComparison.OrdinalIgnoreCase)
                    .Replace("LAG_PLACEHOLDER", model.Lag, StringComparison.OrdinalIgnoreCase);
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
                }, new QuantizeSettings()
                {
                    DitherMethod = DitherMethod.No
                });
            im.Quality = 100;
            return File(im.ToByteArray(MagickFormat.Jpg), "image/jpeg");
        }

        private void Page_Console(object sender, ConsoleEventArgs e)
        {
            throw new NotImplementedException();
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
