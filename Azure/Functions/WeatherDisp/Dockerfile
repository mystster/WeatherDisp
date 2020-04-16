FROM mcr.microsoft.com/dotnet/core/sdk:3.1 AS installer-env

COPY Functions/WeatherDisp/ /src/dotnet-function-app
RUN cd /src/dotnet-function-app && \
    mkdir -p /home/site/wwwroot && \
    dotnet publish *.csproj --output /home/site/wwwroot

FROM node:13.10.1-alpine3.10 AS node-builder
COPY Vue/weatherdisp/ /src
RUN cd /src && \
yarn install && \
yarn run build

# To enable ssh & remote debugging on app service change the base image to the one below
# FROM mcr.microsoft.com/azure-functions/dotnet:3.0-appservice
# FROM mcr.microsoft.com/azure-functions/dotnet:3.0
FROM mystster.azurecr.io/headlesschrome:latest

ENV AzureWebJobsScriptRoot=/home/site/wwwroot \
    AzureFunctionsJobHost__Logging__Console__IsEnabled=true \
    PUPPETEER_EXECUTABLE_PATH=/usr/bin/google-chrome-unstable

COPY --from=installer-env ["/home/site/wwwroot", "/home/site/wwwroot"]
COPY --from=node-builder ["/src/dist", "/home/site/wwwroot/dist"]