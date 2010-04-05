// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GTK_TAB_CONTENTS_DRAG_SOURCE_H_
#define CHROME_BROWSER_GTK_TAB_CONTENTS_DRAG_SOURCE_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/message_loop.h"
#include "base/string16.h"
#include "gfx/point.h"
#include "gfx/native_widget_types.h"
#include "googleurl/src/gurl.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/WebKit/WebKit/chromium/public/WebDragOperation.h"

class TabContents;
class TabContentsView;
struct WebDropData;

// TabContentsDragSource takes care of managing the drag from a TabContents
// with Gtk.
class TabContentsDragSource : public MessageLoopForUI::Observer {
 public:
  explicit TabContentsDragSource(TabContentsView* tab_contents_view);
  ~TabContentsDragSource();

  TabContents* tab_contents() const;

  // Starts a drag for the tab contents this TabContentsDragSource was
  // created for.
  void StartDragging(const WebDropData& drop_data,
                     WebKit::WebDragOperationsMask allowed_ops,
                     GdkEventButton* last_mouse_down,
                     const SkBitmap& image,
                     const gfx::Point& image_offset);

  // MessageLoop::Observer implementation:
  virtual void WillProcessEvent(GdkEvent* event);
  virtual void DidProcessEvent(GdkEvent* event);

 private:
  static gboolean OnDragFailedThunk(GtkWidget* widget,
                                    GdkDragContext* drag_context,
                                    GtkDragResult result,
                                    TabContentsDragSource* handler) {
    return handler->OnDragFailed();
  }
  gboolean OnDragFailed();
  static void OnDragBeginThunk(GtkWidget* widget,
                             GdkDragContext* drag_context,
                             TabContentsDragSource* handler) {
    handler->OnDragBegin(drag_context);
  }
  void OnDragBegin(GdkDragContext* drag_context);
  static void OnDragEndThunk(GtkWidget* widget,
                             GdkDragContext* drag_context,
                             TabContentsDragSource* handler) {
    handler->OnDragEnd(drag_context, drag_context->action);
  }
  void OnDragEnd(GdkDragContext* drag_context,
                 GdkDragAction operation);
  static void OnDragDataGetThunk(GtkWidget* drag_widget,
                                 GdkDragContext* context,
                                 GtkSelectionData* selection_data,
                                 guint target_type,
                                 guint time,
                                 TabContentsDragSource* handler) {
    handler->OnDragDataGet(context, selection_data, target_type, time);
  }
  void OnDragDataGet(GdkDragContext* context, GtkSelectionData* selection_data,
                     guint target_type, guint time);

  gfx::NativeView GetContentNativeView() const;

  // The view we're manging the drag for.
  TabContentsView* tab_contents_view_;

  // The drop data for the current drag (for drags that originate in the render
  // view). Non-NULL iff there is a current drag.
  scoped_ptr<WebDropData> drop_data_;

  // The image used for depicting the drag, and the offset between the cursor
  // and the top left pixel.
  SkBitmap drag_image_;
  gfx::Point image_offset_;

  // The mime type for the file contents of the current drag (if any).
  GdkAtom drag_file_mime_type_;

  // Whether the current drag has failed. Meaningless if we are not the source
  // for a current drag.
  bool drag_failed_;

  // This is the widget we use to initiate drags. Since we don't use the
  // renderer widget, we can persist drags even when our contents is switched
  // out.
  GtkWidget* drag_widget_;

  // The file mime type for a drag-out download.
  string16 wide_download_mime_type_;

  // The file name to be saved to for a drag-out download.
  FilePath download_file_name_;

  // The URL to download from for a drag-out download.
  GURL download_url_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsDragSource);
};

#endif  // CHROME_BROWSER_GTK_TAB_CONTENTS_DRAG_SOURCE_H_
