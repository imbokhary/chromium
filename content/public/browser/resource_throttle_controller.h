// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RESOURCE_THROTTLE_CONTROLLER_H_
#define CONTENT_PUBLIC_BROWSER_RESOURCE_THROTTLE_CONTROLLER_H_

namespace content {

// Used by ResourceThrottle implementations to either resume a deferred
// resource load or cancel a resource load at any time.
class ResourceThrottleController {
 public:
  virtual void Cancel() = 0;
  virtual void Resume() = 0;
 protected:
  virtual ~ResourceThrottleController() {}
};

}  // namespace content

#endif  // CONTENT_PUBLIC_BROWSER_RESOURCE_THROTTLE_CONTROLLER_H_
