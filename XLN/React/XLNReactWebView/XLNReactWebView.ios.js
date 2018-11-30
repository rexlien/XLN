/**
 * @providesModule XLNReactWebView
 * @flow
 */
'use strict';

var NativeXLNReactWebView = require('NativeModules').XLNReactWebView;

/**
 * High-level docs for the XLNReactWebView iOS API can be written here.
 */

var XLNReactWebView = {
  test: function() {
    NativeXLNReactWebView.test();
  }
};

module.exports = XLNReactWebView;
