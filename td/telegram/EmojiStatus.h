//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2022
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/td_api.h"
#include "td/telegram/telegram_api.h"

#include "td/utils/common.h"
#include "td/utils/Promise.h"
#include "td/utils/StringBuilder.h"
#include "td/utils/tl_helpers.h"

namespace td {

class Td;

class EmojiStatus {
  int64 custom_emoji_id_ = 0;
  int32 until_date_ = 0;

  friend bool operator==(const EmojiStatus &lhs, const EmojiStatus &rhs);

  friend StringBuilder &operator<<(StringBuilder &string_builder, const EmojiStatus &contact);

 public:
  EmojiStatus() = default;

  EmojiStatus(const td_api::object_ptr<td_api::premiumStatus> &premium_status, int32 duration);

  explicit EmojiStatus(tl_object_ptr<telegram_api::EmojiStatus> &&emoji_status);

  tl_object_ptr<telegram_api::EmojiStatus> get_input_emoji_status() const;

  td_api::object_ptr<td_api::premiumStatus> get_premium_status_object() const;

  int64 get_effective_custom_emoji_id(bool is_premium, int32 unix_time) const;

  bool is_empty() const {
    return custom_emoji_id_ == 0;
  }

  int32 get_until_date() const {
    return until_date_;
  }

  void clear_until_date() {
    until_date_ = 0;
  }

  template <class StorerT>
  void store(StorerT &storer) const {
    bool has_until_date = until_date_ != 0;
    BEGIN_STORE_FLAGS();
    STORE_FLAG(has_until_date);
    END_STORE_FLAGS();
    td::store(custom_emoji_id_, storer);
    if (has_until_date) {
      td::store(until_date_, storer);
    }
  }

  template <class ParserT>
  void parse(ParserT &parser) {
    bool has_until_date;
    BEGIN_PARSE_FLAGS();
    PARSE_FLAG(has_until_date);
    END_PARSE_FLAGS();
    td::parse(custom_emoji_id_, parser);
    if (has_until_date) {
      td::parse(until_date_, parser);
    }
  }
};

inline bool operator==(const EmojiStatus &lhs, const EmojiStatus &rhs) {
  return lhs.custom_emoji_id_ == rhs.custom_emoji_id_ && lhs.until_date_ == rhs.until_date_;
}

inline bool operator!=(const EmojiStatus &lhs, const EmojiStatus &rhs) {
  return !(lhs == rhs);
}

StringBuilder &operator<<(StringBuilder &string_builder, const EmojiStatus &emoji_status);

void get_default_emoji_statuses(Td *td, Promise<td_api::object_ptr<td_api::premiumStatuses>> &&promise);

void get_recent_emoji_statuses(Td *td, Promise<td_api::object_ptr<td_api::premiumStatuses>> &&promise);

void add_recent_emoji_status(EmojiStatus emoji_status);

void clear_recent_emoji_statuses(Td *td, Promise<Unit> &&promise);

}  // namespace td
