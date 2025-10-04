/*
 * Copyright (c) 2025, Niccolo Antonelli-Dziri <niccolo.antonelli-dziri@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibJS/Runtime/Realm.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/NotificationPrototype.h>
#include <LibWeb/HTML/StructuredSerialize.h>
#include <LibWeb/NotificationsAPI/Notification.h>

namespace Web::NotificationsAPI {

GC_DEFINE_ALLOCATOR(Notification);

Notification::Notification(JS::Realm& realm)
    : DOM::EventTarget(realm)
    , m_origin({})
{
}

WebIDL::ExceptionOr<GC::Ref<Notification>> Notification::create(
    JS::Realm& realm,
    String title,
    Optional<NotificationOptions> options,
    URL::Origin origin,
    URL::URL base_URL,
    HighResolutionTime::EpochTimeStamp fallbackTimestamp)
{
    // 1. Let notification be a new notification.
    auto notification = realm.create<Notification>(realm);

    // 2. If options["silent"] is true and options["vibrate"] exists, then throw a TypeError.
    // FIXME: add the condition for options["vibrate"] when VibratePattern is implemented

    // 3. If options["renotify"] is true and options["tag"] is the empty string, then throw a TypeError.
    if (options->renotify == true && options->tag.is_empty())
        return WebIDL::SimpleException { WebIDL::SimpleExceptionType::TypeError, "options[\"tag\"] cannot be the empty string when options[\"renotify\"] is set to true."sv };

    // 4. Set notification’s data to StructuredSerializeForStorage(options["data"]).
    notification->m_data = HTML::structured_serialize_for_storage(realm.vm(), options->data).release_value();

    // 5. Set notification’s title to title.
    notification->m_title = move(title);

    // 6. Set notification’s direction to options["dir"].
    notification->m_direction = options->dir;

    // 7. Set notification’s language to options["lang"].
    notification->m_language = options->lang;

    // 8. Set notification’s origin to origin.
    notification->m_origin = move(origin);

    // 9. Set notification’s body to options["body"].
    notification->m_body = options->body;

    auto try_set_URL = [&base_URL](String const& url) -> Optional<URL::URL> {
        auto maybe_URL = base_URL.complete_url(url);
        if (maybe_URL.has_value())
            return maybe_URL.release_value();
        return {};
    };

    // 10. If options["navigate"] exists, then parse it using baseURL, and if that does not return failure, set notification’s navigation URL to the return value. (Otherwise notification’s navigation URL remains null.)
    if (options->navigate.has_value()) {
        notification->m_navigation_URL = try_set_URL(options->navigate.value());
    }

    // 11. Set notification’s tag to options["tag"].
    notification->m_tag = options->tag;

    // 12. If options["image"] exists, then parse it using baseURL, and if that does not return failure, set notification’s image URL to the return value. (Otherwise notification’s image URL is not set.)
    if (options->image.has_value()) {
        notification->m_image_URL = try_set_URL(options->image.value());
    }

    // 13. If options["icon"] exists, then parse it using baseURL, and if that does not return failure, set notification’s icon URL to the return value. (Otherwise notification’s icon URL is not set.)
    if (options->icon.has_value()) {
        notification->m_icon_URL = try_set_URL(options->icon.value());
    }

    // 14. If options["badge"] exists, then parse it using baseURL, and if that does not return failure, set notification’s badge URL to the return value. (Otherwise notification’s badge URL is not set.)
    if (options->badge.has_value()) {
        notification->m_badge_URL = try_set_URL(options->badge.value());
    }

    // 15. If options["vibrate"] exists, then validate and normalize it and set notification’s vibration pattern to the return value.
    // FIXME

    // 16. If options["timestamp"] exists, then set notification’s timestamp to the value. Otherwise, set notification’s timestamp to fallbackTimestamp.
    if (options->timestamp.has_value())
        notification->m_timestamp = options->timestamp.value();
    else
        notification->m_timestamp = fallbackTimestamp;

    // 17. Set notification’s renotify preference to options["renotify"].
    notification->m_renotify_preference = options->renotify;

    // 18. Set notification’s silent preference to options["silent"].
    if (options->silent.has_value()) {
        dbgln("Has value");
        if (options->silent == false) {
            dbgln("False");
        } else {
            dbgln("True");
        }
    } else {
        dbgln("No value");
    }
    notification->m_silent_preference = options->silent;

    // 19. Set notification’s require interaction preference to options["requireInteraction"].
    notification->m_require_interaction_preference = options->require_interaction;

    // 20. Set notification’s actions to « ».
    notification->m_actions = {};

    // 21. For each entry in options["actions"], up to the maximum number of actions supported (skip any excess entries):
    for (auto entry : options->actions) {
        // FIXME: stop the loop at the max number of actions supported

        // 1. Let action be a new notification action.
        Action action;

        // 2. Set action’s name to entry["action"].
        action.name = entry.action;

        // 3. Set action’s title to entry["title"].
        action.title = entry.title;

        // 4. If entry["navigate"] exists, then parse it using baseURL, and if that does not return failure, set action’s navigation URL to the return value. (Otherwise action’s navigation URL remains null.)
        if (entry.navigate.has_value())
            action.navigation_URL = try_set_URL(entry.navigate.value());

        // 5. If entry["icon"] exists, then parse it using baseURL, and if that does not return failure, set action’s icon URL to the return value. (Otherwise action’s icon URL remains null.)
        if (entry.icon.has_value())
            action.icon_URL = try_set_URL(entry.icon.value());

        // 6. Append action to notification’s actions.
        notification->m_actions.append(action);
    }

    // 22. Return notification.
    return notification;
}

WebIDL::ExceptionOr<GC::Ref<Notification>> Notification::create(
    JS::Realm& realm,
    String title,
    Optional<NotificationOptions> options,
    GC::Ptr<HTML::EnvironmentSettingsObject> settings)
{
    // 1. Let origin be settings’s origin.
    URL::Origin origin = settings->origin();

    // 2. Let baseURL be settings’s API base URL.
    URL::URL base_URL = settings->api_base_url();

    // 3. Let fallbackTimestamp be the number of milliseconds from the Unix epoch to settings’s current wall time, rounded to the nearest integer.
    HighResolutionTime::EpochTimeStamp fallbacktimestamp = 0; // FIXME: set fallbackTimestamp to settings to correct value

    // 4. Return the result of creating a notification given title, options, origin, baseURL, and fallbackTimestamp.
    return create(realm, move(title), options, origin, base_URL, fallbacktimestamp);
}

// https://notifications.spec.whatwg.org/#constructors
WebIDL::ExceptionOr<GC::Ref<Notification>> Notification::construct_impl(
    JS::Realm& realm,
    String title,
    Optional<NotificationOptions> options)
{

    Notification notif(realm);

    // auto& relevant_global_object = HTML::relevant_global_object(notification);
    auto& relevant_settings_object = HTML::relevant_settings_object(notif);

    // 1. If this’s relevant global object is a ServiceWorkerGlobalScope object, then throw a TypeError.
    // FIXME

    // 2. If options["actions"] is not empty, then throw a TypeError.
    if (!options->actions.is_empty())
        return WebIDL::SimpleException { WebIDL::SimpleExceptionType::TypeError, "Options `action` is not empty"sv };

    // 3. Let notification be the result of creating a notification with a settings object given title, options, and this’s relevant settings object.
    auto notification = create(realm, move(title), options, relevant_settings_object);

    // 4. Associate this with notification.
    // NOTE: Does this mean merging the two notifications into one ?

    // 5. Run these steps in parallel:

    // 1. If the result of getting the notifications permission state is not "granted", then queue a task to fire an event named error on this, and abort these steps.

    // 2. Run the notification show steps for notification.

    return notification;
}

void Notification::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(Notification);
    Base::initialize(realm);
}

}
