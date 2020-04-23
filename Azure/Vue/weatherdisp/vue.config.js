module.exports = {
    publicPath: process.env.NODE_ENV === 'production'
        ? '.'
        : '/',
    devServer: {
        watchOptions: {
            poll: true
        }
    },
    css: {
        extract: false,
    },
    configureWebpack: {
        optimization: {
            splitChunks: false
        },
    },
    chainWebpack: config => {
        config.plugins.delete('prefetch')
        config.plugins.delete('preload')
        config
            .plugin('html')
            .tap(args => {
                args[0].inlineSource = '.(js|css|map)$';
                return args
            })
        config
            .plugin('html-inline-source')
            .use(require('html-webpack-inline-source-plugin'))
        config.module
            .rule('images')
            .use('url-loader')
            .options({})
        config.module
            .rule('replace')
            .test(/.*darksky\-api\.es5\.js$/)
            .use('string-replace-loader')
            .loader('string-replace-loader')
            .options({
                search: /return 'colors' in exports\.inspectOpts\s+\? Boolean\(exports\.inspectOpts\.colors\)\s+: tty\.isatty\(process\.stderr\.fd\);/gim,
                replace: 'return false'
            })
    }
};
