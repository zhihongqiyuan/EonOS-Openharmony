/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// Generated using webpack-cli https://github.com/webpack/webpack-cli

const path = require('path');
const { CleanWebpackPlugin } = require('clean-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const isProduction = process.env.NODE_ENV === 'production';
const CopyWebpackPlugin = require('copy-webpack-plugin');
const os = require('os');
const childProcess = require('child_process');
const { exec } = require('child_process');
const fs = require('fs');

const supportPlatform = ['windows', 'linux', 'darwin'];

function runCommand(command) {
  return new Promise((resolve, reject) => {
    exec(command, (error, stdout, stderr) => {
      if (error) {
        reject(error);
      } else {
        resolve(stdout);
      }
    });
  });
}

function cpFile(sourcePath, targetPath) {
  fs.readdir(sourcePath, (err, files) => {
    if (err) {
      console.error('无法读取目录', err);
      return;
    }
    files.forEach((file) => {
      const source = `${sourcePath}/${file}`;
      const target = `${targetPath}/${file}`;
      if (fs.lstatSync(source).isFile()) {
        fs.copyFile(source, target, (err) => {
          if (err) {
            console.error('无法复制文件', err);
            return;
          }
        });
      }
    });
  });
}

function clearDirectory(directoryPath) {
  const isDirectoryExists = fs.existsSync(directoryPath);

  if (!isDirectoryExists) {
    fs.mkdirSync(directoryPath);
  } else {
    fs.readdirSync(directoryPath).forEach((file) => {
      const filePath = path.join(directoryPath, file);
      if (fs.lstatSync(filePath).isDirectory()) {
        return;
      } else {
        try {
          fs.unlinkSync(filePath); // 删除文件
        } catch {
        }
      }
    });
  }
}

function buildMultiPlatform() {
  const outPath = path.normalize(path.join(__dirname, '/', 'dist'));
  const serverSrc = path.normalize(path.join(__dirname, '/server/main.go'));
  for (const platform of supportPlatform) {
    const generateFile = platform === 'windows' ?
      path.normalize(path.join(outPath, '/', `main.exe`)) :
      path.normalize(path.join(outPath, '/', `main_${platform}`));
    setEnv = '';
    if (os.type() === 'Windows_NT') {
      setEnv += `SET CGO_ENABLED=0&&SET GOOS=${platform}&&SET GOARCH=amd64&&`;
    } else {
      setEnv += `CGO_ENABLED=0 GOOS=${platform} GOARCH=amd64`;
    }
    const buildCmd = `${setEnv} go build -o ${generateFile} ${serverSrc}`;
    console.log(`compile ${platform} server ...`);
    childProcess.execSync(buildCmd);
  }
}

const stylesHandler = isProduction ? MiniCssExtractPlugin.loader : 'style-loader';
//compile server
((flag) => {
  if (!flag) {
    return;
  }
  console.log('start compile server');
  let outPath = path.normalize(path.join(__dirname, '/', 'dist'));
  let binPath = path.normalize(path.join(__dirname, '/', 'bin'));
  clearDirectory(outPath);
  cpFile(binPath, outPath);
  const protoPath = './src/trace/proto/';
  runCommand(`pbjs -t static-module -w commonjs -o ${protoPath}SphBaseData.js  ${protoPath}SphBaseData.proto`);
  buildMultiPlatform();
})(true);
const config = {
  entry: './src/index.ts',
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: '[name].[chunkhash].bundle.js',
    clean: false,
  },
  devServer: {
    open: true,
    host: 'localhost',
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: 'index.html',
    }),
    new CleanWebpackPlugin({
      verbose: true,
      cleanOnceBeforeBuildPatterns: ['!main'],
    }),
    new CopyWebpackPlugin({
      patterns: [
        {
          from: './src/figures',
          to: 'figures',
        },
        {
          from: './src/img',
          to: 'img',
        },
        {
          from: './src/doc',
          to: 'doc',
        },
        {
          from: './src/base-ui/icon.svg',
          to: 'base-ui/icon.svg',
        },
        {
          from: './src/trace/config/custom_temp_config.json',
          to: 'trace/config/custom_temp_config.json',
        },
        {
          from: './third-party/sql-wasm.js',
          to: 'sql-wasm.js',
        },
        {
          from: './third-party/sql-wasm.wasm',
          to: 'sql-wasm.wasm',
        },
        {
          from: './server/version.txt',
          to: 'version.txt',
        },
        {
          from: './server/wasm.json',
          to: 'wasm.json',
        },
        {
          from: './server/server-config.json',
          to: 'server-config.json',
        },
      ],
    }),
    // Add your plugins here
    // Learn more about plugins from https://webpack.js.org/configuration/plugins/
  ],
  module: {
    rules: [
      {
        test: /\.(ts|tsx)$/i,
        loader: 'ts-loader',
        exclude: ['/node_modules/'],
      },
      {
        test: /\.css$/i,
        use: [stylesHandler, 'css-loader', 'postcss-loader'],
      },
      {
        test: /\.(eot|svg|ttf|woff|woff2|png|jpg|gif)$/i,
        type: 'asset',
      },

      // Add your rules for custom modules here
      // Learn more about loaders from https://webpack.js.org/loaders/
    ],
  },
  resolve: {
    extensions: ['.tsx', '.ts', '.jsx', '.js', '...'],
    fallback: {
      fs: false,
      tls: false,
      net: false,
      zlib: false,
      http: false,
      https: false,
      stream: false,
      crypto: false,
      path: false, //if you want to use this module also don't forget npm i crypto-browserify
    },
  },
};

module.exports = () => {
  if (isProduction) {
    config.mode = 'production';

    config.plugins.push(new MiniCssExtractPlugin());
  } else {
    config.mode = 'development';
  }
  return config;
};
