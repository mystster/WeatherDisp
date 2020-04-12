module.exports = {
    publicPath: process.env.NODE_ENV === 'production'
        ? '.'
        : '/',
    devServer: {
        watchOptions: {
            poll: true
        }
    }
};
