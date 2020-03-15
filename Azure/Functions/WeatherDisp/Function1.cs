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

namespace WeatherDisp
{
    public static class Function1
    {
        [FunctionName("Function1")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", "post", Route = null)] HttpRequest req,
            ILogger log, Microsoft.Azure.WebJobs.ExecutionContext context)
        {
            log.LogInformation("C# HTTP trigger function processed a request.");

            //string name = req.Query["name"];

            //string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
            //dynamic data = JsonConvert.DeserializeObject(requestBody);
            //name = name ?? data?.name;

            //return name != null
            //   ? (ActionResult)new OkObjectResult($"Hello, {name}")
            //   : new BadRequestObjectResult("Please pass a name on the query string or in the request body");

            using (var browser = await Puppeteer.LaunchAsync(new LaunchOptions()
            {
                Headless = true,
                LogProcess = true,
                Args = new[]
                {
                    "--no-sandbox"
                }
            }))
            {
                using (var page = await browser.NewPageAsync())
                {
                    await page.SetViewportAsync(new ViewPortOptions()
                    {
                        Width = 298,
                        Height = 128
                    });
                    
                    //await page.GoToAsync("https://www.google.com");
                    using(var fs = new StreamReader(Path.Combine(context.FunctionAppDirectory, "test.html")))
                    {
                        await page.SetContentAsync(fs.ReadToEnd());
                    }
                    var aaa = await page.GetContentAsync();
                    return new FileContentResult(await page.ScreenshotDataAsync(), "image/jpeg");
                }
            }
        }
    }
}
