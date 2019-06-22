const path = require("path");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CopyPlugin = require("copy-webpack-plugin");
const contentBase = path.join(__dirname, "/public");
const buildPath = path.join(__dirname, "/build");

const isProd = process.env.NODE_ENV === "production";

module.exports = {
  entry: "./src/Index.bs.js",
  mode: isProd ? "production" : "development",
  output: {
    path: buildPath,
    filename: "Index.js"
  },
  module: {
    rules: [
      {
        test: /\.css$/,
        use: ["style-loader", "css-loader", "postcss-loader"]
        // use: [
        //   MiniCssExtractPlugin.loader,
        //   "style-loader",
        //   "css-loader",
        //   "postcss-loader"
        // ]
      }
    ]
  },
  plugins: [
    new MiniCssExtractPlugin({
      filename: "styles.css",
      chunkFilename: "styles.css"
    }),
    new HtmlWebpackPlugin({
      template: "src/index.html",
      inject: false
    }),
    new CopyPlugin([{ from: "public" }])
  ],
  devServer: {
    compress: true,
    contentBase,
    port: process.env.PORT || 8000,
    historyApiFallback: true
  }
};
