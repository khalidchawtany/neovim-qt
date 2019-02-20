#include "OSXHideTitleBar.h"
#include <Cocoa/Cocoa.h>

void OSXHideTitleBar::HideTitleBar(long winid)
{
    NSView *nativeView = reinterpret_cast<NSView *>(winid);
    NSWindow* nativeWindow = [nativeView window];

    [nativeWindow setStyleMask:
        [nativeWindow styleMask] | NSFullSizeContentViewWindowMask | NSWindowTitleHidden];

    [nativeWindow setTitlebarAppearsTransparent:YES];
    [nativeWindow setMovableByWindowBackground:YES];

    NSButton *closeButton = [nativeWindow standardWindowButton:NSWindowCloseButton];
    NSButton *minButton = [nativeWindow standardWindowButton:NSWindowMiniaturizeButton];
    NSButton *zoomButton = [nativeWindow standardWindowButton:NSWindowZoomButton];
    [closeButton setHidden:YES];
    [minButton setHidden:YES];
    [zoomButton setHidden:YES];
}
