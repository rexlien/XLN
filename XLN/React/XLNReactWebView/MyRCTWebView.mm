/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "MyRCTWebView.h"

#import <UIKit/UIKit.h>

#import "React/RCTAutoInsetsProtocol.h"
#import "React/RCTConvert.h"
#import "React/RCTEventDispatcher.h"
#import "React/RCTLog.h"
#import "React/RCTUtils.h"
#import "React/RCTView.h"
#import "React/UIView+React.h"

#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Framework/ExecutorService.h>
#include <XLN/Core/Framework/RenderFrame.h>

#include <XLN/Core/Framework/Application.h>
#include <XLN/Core/Framework/Message/WebViewMessage.h>
#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/EventHandler.h>

//class MyHandler : public XCr::EventHandler
//{
//public:
//    
//    MyReactWebView *webView;
//    virtual void OnHandleMessage(XCr::SharedPtr<XCr::IMessage> message)
//    {   /*
//        if(message->ObjClassID() == XGf::WebViewMessage::ClassID()) {
//            
//            XCr::SharedPtr<XGf::WebViewMessage> webViewMsg = std::static_pointer_cast<XGf::WebViewMessage>(message);
//            const XCr::String cid = webViewMsg->GetWebViewID();
//            const XCr::String viewID = [[[webView reactTag] stringValue] UTF8String];
//            if(cid == viewID)
//            {
//                XGf::WebViewMessage::Command  command = webViewMsg->GetCommand();
//                if(command == XGf::WebViewMessage::INJECT_JSCRIPT)
//                {
//                    const XCr::String *script = webViewMsg->GetValue("script");
//                    NSString* nsScript = [NSString stringWithUTF8String:script->c_str()];
//                    dispatch_sync(dispatch_get_main_queue(), ^{
//                        
//                        [webView injectJavaScript:nsScript];
//                        
//                    });
//                   
//                }
//            }
//        }
//         */
//    }
//    
//};
//

@interface xln_react_WebViewManager : NSObject


@property (class, nonatomic, assign) MyReactWebView* webView;

+ (void)callJavaScript:(NSString*)viewID script:(NSString*)script;


@end

@implementation xln_react_WebViewManager

static MyReactWebView* _webView;

+ (MyReactWebView*)webView {
    return _webView;
}

+ (void)setWebView:(MyReactWebView*)newValue {
    _webView = newValue;
}

+(void)callJavaScript:(NSString*)viewID script:(NSString*)script
{
    const XCr::String cid = [viewID UTF8String];
    const XCr::String webViewID = [[[_webView reactTag] stringValue] UTF8String];
    if(cid == webViewID)
    {
        
        dispatch_sync(dispatch_get_main_queue(), ^{
            
            [_webView injectJavaScript:script];
            
        });
        
    }

}
@end



@interface MyReactWebView () <UIWebViewDelegate, RCTAutoInsetsProtocol>
{
    //XCr::SharedPtr<MyHandler> m_EventHandler;
    

}

@property (nonatomic, copy) RCTDirectEventBlock onLoadingStart;
@property (nonatomic, copy) RCTDirectEventBlock onLoadingFinish;
@property (nonatomic, copy) RCTDirectEventBlock onLoadingError;
@property (nonatomic, copy) RCTDirectEventBlock onShouldStartLoadWithRequest;
@property (nonatomic, copy) RCTDirectEventBlock onMessage;

@end

@implementation MyReactWebView
{
  UIWebView *_webView;
  //NSString *_injectedJavaScript;
}


- (id)init
{
    self = [super init];
    if(self)
    {
        XGf::ExecutorService* executorService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ExecutorService>();
        if(executorService)
        {
            //m_EventHandler = executorService->GetRenderFrame()->AcquireEventHandler<MyHandler>();
            //m_EventHandler->webView = self;
        }

    }
    return self;
}

- (void)dealloc
{
  //m_EventHandler->webView = 0;
  _webView.delegate = nil;
  xln_react_WebViewManager.webView = nil;
}

- (instancetype)initWithFrame:(CGRect)frame
{
  if ((self = [super initWithFrame:frame])) {
    super.backgroundColor = [UIColor clearColor];
    _webView = [[UIWebView alloc] initWithFrame:self.bounds];
    _webView.delegate = self;
    xln_react_WebViewManager.webView = self;
    
    [self addSubview:_webView];
  }
  return self;
}

/*
RCT_NOT_IMPLEMENTED(- (instancetype)initWithCoder:(NSCoder *)aDecoder)

- (void)goForward
{
  [_webView goForward];
}

- (void)goBack
{
  [_webView goBack];
}

- (void)reload
{
  NSURLRequest *request = [RCTConvert NSURLRequest:self.source];
  if (request.URL && !_webView.request.URL.absoluteString.length) {
    [_webView loadRequest:request];
  }
  else {
    [_webView reload];
  }
}

- (void)stopLoading
{
  [_webView stopLoading];
}

- (void)postMessage:(NSString *)message
{
  NSDictionary *eventInitDict = @{
    @"data": message,
  };
  NSString *source = [NSString
    stringWithFormat:@"document.dispatchEvent(new MessageEvent('message', %@));",
    RCTJSONStringify(eventInitDict, NULL)
  ];
  [_webView stringByEvaluatingJavaScriptFromString:source];
}

- (void)injectJavaScript:(NSString *)script
{
  [_webView stringByEvaluatingJavaScriptFromString:script];
}

- (void)layoutSubviews
{
  [super layoutSubviews];
  _webView.frame = self.bounds;
}



- (void)setScalesPageToFit:(BOOL)scalesPageToFit
{
  if (_webView.scalesPageToFit != scalesPageToFit) {
    _webView.scalesPageToFit = scalesPageToFit;
    [_webView reload];
  }
}

- (BOOL)scalesPageToFit
{
  return _webView.scalesPageToFit;
}

- (void)setBackgroundColor:(UIColor *)backgroundColor
{
  CGFloat alpha = CGColorGetAlpha(backgroundColor.CGColor);
  self.opaque = _webView.opaque = (alpha == 1.0);
  _webView.backgroundColor = backgroundColor;
}

- (UIColor *)backgroundColor
{
  return _webView.backgroundColor;
}

- (NSMutableDictionary<NSString *, id> *)baseEvent
{
  NSMutableDictionary<NSString *, id> *event = [[NSMutableDictionary alloc] initWithDictionary:@{
    @"url": _webView.request.URL.absoluteString ?: @"",
    @"loading" : @(_webView.loading),
    @"title": [_webView stringByEvaluatingJavaScriptFromString:@"document.title"],
    @"canGoBack": @(_webView.canGoBack),
    @"canGoForward" : @(_webView.canGoForward),
  }];

  return event;
}

- (void)refreshContentInset
{
  [RCTView autoAdjustInsetsForView:self
                    withScrollView:_webView.scrollView
                      updateOffset:YES];
}
*/

- (NSMutableDictionary<NSString *, id> *)baseEvent
{
  NSMutableDictionary<NSString *, id> *event = [[NSMutableDictionary alloc] initWithDictionary:@{
                                                                                                 @"url": _webView.request.URL.absoluteString ?: @"",
                                                                                                 @"loading" : @(_webView.loading),
                                                                                                 @"title": [_webView stringByEvaluatingJavaScriptFromString:@"document.title"],
                                                                                                 @"canGoBack": @(_webView.canGoBack),
                                                                                                 @"canGoForward" : @(_webView.canGoForward),
                                                                                                 }];
  
  return event;
}

#pragma mark - UIWebViewDelegate methods

- (BOOL)webView:(__unused UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType
{
  
  BOOL shouldUrlNavigate = [request.URL.scheme isEqualToString:@"http"] || [request.URL.scheme isEqualToString:@"https"] || [request.URL.scheme isEqualToString:@"file"];
  BOOL isJSNavigation = [request.URL.scheme isEqualToString:RCTJSNavigationScheme];
  
  if(!shouldUrlNavigate)
  {
    XGf::ExecutorService* execService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ExecutorService>();
    XCr::String urlString([request.URL.absoluteString UTF8String]);
    XCr::String tag([[[self reactTag] stringValue]UTF8String]);
    
    XCr::SharedPtr<XGf::WebViewMessage> webviewMsg = std::make_shared<XGf::WebViewMessage>(tag, XGf::WebViewMessage::Command::LOAD_URL);
    webviewMsg->InsertValue("url", urlString);
    
    execService->PostRenderFrame(XCr::Task::Category::CATEGORY_ALL, std::static_pointer_cast<XCr::IMessage>(webviewMsg));
    //return NO;
    isJSNavigation = true;
      
  }
  

  static NSDictionary<NSNumber *, NSString *> *navigationTypes;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    navigationTypes = @{
      @(UIWebViewNavigationTypeLinkClicked): @"click",
      @(UIWebViewNavigationTypeFormSubmitted): @"formsubmit",
      @(UIWebViewNavigationTypeBackForward): @"backforward",
      @(UIWebViewNavigationTypeReload): @"reload",
      @(UIWebViewNavigationTypeFormResubmitted): @"formresubmit",
      @(UIWebViewNavigationTypeOther): @"other",
    };
  });

  // skip this for the JS Navigation handler
  if (!isJSNavigation && _onShouldStartLoadWithRequest) {
    NSMutableDictionary<NSString *, id> *event = [self baseEvent];
    [event addEntriesFromDictionary: @{
      @"url": (request.URL).absoluteString,
      @"navigationType": navigationTypes[@(navigationType)]
    }];
    if (![self.delegate webView:self
      shouldStartLoadForRequest:event
                   withCallback:_onShouldStartLoadWithRequest]) {
      return NO;
    }
  }

  if (_onLoadingStart) {
    // We have this check to filter out iframe requests and whatnot
    BOOL isTopFrame = [request.URL isEqual:request.mainDocumentURL];
    if (isTopFrame) {
      NSMutableDictionary<NSString *, id> *event = [self baseEvent];
      [event addEntriesFromDictionary: @{
        @"url": (request.URL).absoluteString,
        @"navigationType": navigationTypes[@(navigationType)]
      }];
      _onLoadingStart(event);
    }
  }

  if (isJSNavigation && [request.URL.host isEqualToString:RCTJSPostMessageHost]) {
    NSString *data = request.URL.query;
    data = [data stringByReplacingOccurrencesOfString:@"+" withString:@" "];
    data = [data stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];

    NSMutableDictionary<NSString *, id> *event = [self baseEvent];
    [event addEntriesFromDictionary: @{
      @"data": data,
    }];
    _onMessage(event);
  }

  // JS Navigation handler
  return !isJSNavigation;
   
  
  
}

@end
