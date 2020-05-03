FROM mcr.microsoft.com/dotnet/core/sdk:3.1 AS installer-env

COPY Functions/WeatherDisp/ /src/dotnet-function-app
RUN cd /src/dotnet-function-app && \
    mkdir -p /home/site/wwwroot && \
    dotnet publish *.csproj --output /home/site/wwwroot && \
    cp /home/site/wwwroot/bin/runtimes/linux-x64/native/Magick.Native-Q8-x64.dll.so /home/site/wwwroot/bin/Magick.Native-Q8-x64.dll.so

FROM node:13.10.1-alpine3.10 AS node-builder
COPY Vue/weatherdisp/ /src
RUN cd /src && \
yarn install && \
yarn run build

# To enable ssh & remote debugging on app service change the base image to the one below
# FROM mcr.microsoft.com/azure-functions/dotnet:3.0-appservice
# FROM mcr.microsoft.com/azure-functions/dotnet:3.0
# FROM mystster.azurecr.io/headlesschrome:latest
# To enable ssh & remote debugging on app service change the base image to the one below
# FROM mcr.microsoft.com/azure-functions/dotnet:3.0-appservice
FROM mcr.microsoft.com/azure-functions/dotnet:3.0

# apt-get install 時にダイアログ抑止
ENV DEBIAN_FRONTEND=noninteractive \
    APT_KEY_DONT_WARN_ON_DANGEROUS_USAGE=DontWarn
# いろいろインストール
RUN apt-get update \
    && apt-get -y install --no-install-recommends apt-utils dialog 2>&1 \
    # 下の日本語フォント用
    && apt-get -y install --no-install-recommends wget unzip gnupg2 fontconfig\ 
    # chrome
    && wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add - \
    && sh -c 'echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list' \
    && apt-get update \
    && apt-get install -y google-chrome-unstable fonts-ipafont-gothic fonts-wqy-zenhei fonts-thai-tlwg fonts-kacst fonts-freefont-ttf \
    --no-install-recommends \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/* \
    # 日本語フォントを入れる
    && mkdir /tmp/noto && \
    cd /tmp/noto && \
    wget -q https://noto-website.storage.googleapis.com/pkgs/NotoSansCJKjp-hinted.zip -O NotoSansCJKjp-hinted.zip && \
    unzip NotoSansCJKjp-hinted.zip && \
    mkdir -p /usr/share/fonts/noto && \
    cp *.otf /usr/share/fonts/noto && \
    chmod 644 -R /usr/share/fonts/noto/ && \
    fc-cache -fv&& \
    cd / && \
    rm -rf /temp/noto

ENV AzureWebJobsScriptRoot=/home/site/wwwroot \
    AzureFunctionsJobHost__Logging__Console__IsEnabled=true \
    PUPPETEER_EXECUTABLE_PATH=/usr/bin/google-chrome-unstable

COPY --from=installer-env ["/home/site/wwwroot", "/home/site/wwwroot"]
COPY --from=node-builder ["/src/dist", "/home/site/wwwroot/dist"]