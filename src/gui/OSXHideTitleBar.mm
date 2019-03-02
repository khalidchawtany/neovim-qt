#include "OSXHideTitleBar.h"
#include <Cocoa/Cocoa.h>

void OSXHideTitleBar::HideTitleBar(long winid, bool toggle)
{
    NSView *nativeView = reinterpret_cast<NSView *>(winid);
    NSWindow* nativeWindow = [nativeView window];

    if(toggle) {
        [nativeWindow setStyleMask:
            [nativeWindow styleMask] | NSFullSizeContentViewWindowMask | NSWindowTitleHidden ];

        [nativeWindow setTitlebarAppearsTransparent:YES];
        [nativeWindow setTitleVisibility:NSWindowTitleHidden];
        /* nativeWindow.titleVisibility = NSWindowTitleHidden; */
        /* [nativeWindow setMovableByWindowBackground:YES]; */

        NSButton *closeButton = [nativeWindow standardWindowButton:NSWindowCloseButton];
        NSButton *minButton = [nativeWindow standardWindowButton:NSWindowMiniaturizeButton];
        NSButton *zoomButton = [nativeWindow standardWindowButton:NSWindowZoomButton];
        [closeButton setHidden:YES];
        [minButton setHidden:YES];
        [zoomButton setHidden:YES];
    } else {
        [nativeWindow setStyleMask:
            [nativeWindow styleMask] | NSFullSizeContentViewWindowMask | NSWindowTitleVisible];


        [nativeWindow setTitlebarAppearsTransparent:false];
        /* [nativeWindow setMovableByWindowBackground:false]; */

        NSButton *closeButton = [nativeWindow standardWindowButton:NSWindowCloseButton];
        NSButton *minButton = [nativeWindow standardWindowButton:NSWindowMiniaturizeButton];
        NSButton *zoomButton = [nativeWindow standardWindowButton:NSWindowZoomButton];
        [closeButton setHidden:false];
        [minButton setHidden:false];
        [zoomButton setHidden:false];

    }

}
