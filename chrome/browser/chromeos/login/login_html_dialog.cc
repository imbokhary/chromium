// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/login/login_html_dialog.h"

#include "chrome/browser/chromeos/login/helper.h"
#include "chrome/browser/profile_manager.h"
#include "chrome/browser/views/browser_dialogs.h"
#include "gfx/native_widget_types.h"
#include "gfx/rect.h"
#include "gfx/size.h"
#include "views/window/window.h"

namespace chromeos {

namespace {
// Default width/height ratio of screen size.
const float kDefaultWidthRatio = 0.8;
const float kDefaultHeightRatio = 0.8;
}  // namespace

///////////////////////////////////////////////////////////////////////////////
// LoginHtmlDialog, public:

LoginHtmlDialog::LoginHtmlDialog(Delegate* delegate,
                                 gfx::NativeWindow parent_window,
                                 const std::wstring& title,
                                 const GURL& url)
    : delegate_(delegate),
      parent_window_(parent_window),
      title_(title),
      url_(url) {
  gfx::Rect screen_bounds(chromeos::CalculateScreenBounds(gfx::Size(0, 0)));
  width_ = static_cast<int>(kDefaultWidthRatio * screen_bounds.width());
  height_ = static_cast<int>(kDefaultHeightRatio * screen_bounds.height());
}

LoginHtmlDialog::~LoginHtmlDialog() {
  delegate_ = NULL;
}

void LoginHtmlDialog::Show() {
  browser::ShowHtmlDialogView(parent_window_,
                              ProfileManager::GetDefaultProfile(),
                              this);
}

void LoginHtmlDialog::SetDialogSize(int width, int height) {
  DCHECK(width >= 0 && height >= 0);
  width_ = width;
  height_ = height;
}

///////////////////////////////////////////////////////////////////////////////
// LoginHtmlDialog, protected:

void LoginHtmlDialog::OnDialogClosed(const std::string& json_retval) {
  if (delegate_)
    delegate_->OnDialogClosed();
}

void LoginHtmlDialog::OnCloseContents(TabContents* source,
                                      bool* out_close_dialog) {
  if (out_close_dialog)
    *out_close_dialog = true;
}

void LoginHtmlDialog::GetDialogSize(gfx::Size* size) const {
  size->SetSize(width_, height_);
}

}  // namespace chromeos
