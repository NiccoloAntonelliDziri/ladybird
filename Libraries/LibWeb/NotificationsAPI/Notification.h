/*
}
 * Copyright (c) 2025, Niccolo Antonelli-Dziri <niccolo.antonelli-dziri@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Optional.h>
#include <AK/String.h>
#include <AK/Vector.h>
#include <LibJS/Runtime/Realm.h>
#include <LibJS/Runtime/Value.h>
#include <LibWeb/DOM/EventTarget.h>
#include <LibWeb/HighResolutionTime/EpochTimeStamp.h>

namespace Web::NotificationsAPI {

struct NotificationAction {
    String action;
    String title;
    Optional<String> navigate;
    Optional<String> icon;
};

struct NotificationOptions {
    Bindings::NotificationDirection dir = Bindings::NotificationDirection::Auto;
    String lang = ""_string;
    String body = ""_string;
    Optional<String> navigate;
    String tag = ""_string;
    Optional<String> image;
    Optional<String> icon;
    Optional<String> badge;
    // VibratePattern vibrate;  // FIXME: properly implement vibrate pattern
    Optional<HighResolutionTime::EpochTimeStamp> timestamp;
    bool renotify = false;
    Optional<bool> silent = OptionalNone();
    bool require_interaction = false;
    JS::Value data;
    Vector<NotificationAction> actions;
};

// https://notifications.spec.whatwg.org/#notifications
class WEB_API Notification final : public DOM::EventTarget {
    WEB_PLATFORM_OBJECT(Notification, DOM::EventTarget);
    GC_DECLARE_ALLOCATOR(Notification);

public:
    [[nodiscard]] static WebIDL::ExceptionOr<GC::Ref<Notification>> construct_impl(
        JS::Realm& realm,
        String title,
        Optional<NotificationOptions> options);

    // To create a notification with a settings object, given a string title, NotificationOptions dictionary options, and environment settings object settings:
    // https://notifications.spec.whatwg.org/#create-a-notification-with-a-settings-object
    static WebIDL::ExceptionOr<GC::Ref<Notification>> create(
        JS::Realm& realm,
        String title,
        Optional<NotificationOptions> options,
        GC::Ptr<HTML::EnvironmentSettingsObject> settings);

    // To create a notification, given a string title, NotificationOptions dictionary options, an origin origin, a URL baseURL, and an EpochTimeStamp fallbackTimestamp:
    // https://notifications.spec.whatwg.org/#create-a-notification
    static WebIDL::ExceptionOr<GC::Ref<Notification>> create(
        JS::Realm& realm,
        String title,
        Optional<NotificationOptions> options,
        URL::Origin origin,
        URL::URL base_URL,
        HighResolutionTime::EpochTimeStamp fallbackTimestamp);

    String title() const { return this->m_title; }
    Bindings::NotificationDirection dir() const { return this->m_direction; }
    String lang() const { return this->m_language; }
    String body() const { return this->m_body; }
    String navigate() const { return this->m_navigation_URL.has_value() ? this->m_navigation_URL->serialize() : ""_string; }
    String tag() const { return this->m_tag; }
    String image() const { return this->m_image_URL.has_value() ? this->m_image_URL->serialize() : ""_string; }
    String icon() const { return this->m_icon_URL.has_value() ? this->m_icon_URL->serialize() : ""_string; }
    String badge() const { return this->m_badge_URL.has_value() ? this->m_badge_URL->serialize() : ""_string; }
    HighResolutionTime::EpochTimeStamp timestamp() const { return this->m_timestamp; }
    bool renotify() const { return this->m_renotify_preference; }
    Optional<bool> silent() const { return this->m_silent_preference; }
    bool require_interaction() const { return this->m_require_interaction_preference; }
    // FIXME: data method
    // FIXME: action method

private:
    Notification(JS::Realm&);

    virtual void initialize(JS::Realm&) override;

    HTML::SerializationRecord m_data;
    String m_title;
    Bindings::NotificationDirection m_direction;
    String m_language;
    URL::Origin m_origin;
    String m_body;
    Optional<URL::URL> m_navigation_URL;
    String m_tag;
    Optional<URL::URL> m_image_URL;
    Optional<URL::URL> m_icon_URL;
    Optional<URL::URL> m_badge_URL;

    // FIXME: add the ressources from m_image_URL, m_icon_URL and m_badge_URL

    HighResolutionTime::EpochTimeStamp m_timestamp;
    bool m_renotify_preference;
    Optional<bool> m_silent_preference;
    bool m_require_interaction_preference;

    // https://notifications.spec.whatwg.org/#action
    struct Action {
        String name;
        String title;
        Optional<URL::URL> navigation_URL;
        Optional<URL::URL> icon_URL;
        // FIXME: icon ressource
    };
    // A notification has associated actions (a list of zero or more notification actions). A notification action represents a choice for an end user. Each notification action has an associated:
    Vector<Action> m_actions;
};

}
