#pragma once

/**
 * @file    types.hpp
 * @brief   Common fundamental and utility types used across the engine
 * @author  alex-1-tech
 * @date    2026
 */


#include "engine/core/base.hpp"
#include "engine/core/math.hpp"

EGE_NAMESPACE_BEGIN

static constexpr u8 MAX_VALUE = 255;
static constexpr u8 GRAY_VALUE = 128;
static constexpr u8 DARK_GRAY_VALUE = 64;

class Color
{
public:
  constexpr Color() = default;

  static constexpr auto rgba(u8 red, u8 green, u8 blue, u8 alpha = MAX_VALUE) -> Color { return {red, green, blue, alpha}; }

  static constexpr auto rgb(u8 red, u8 green, u8 blue) -> Color { return {red, green, blue, MAX_VALUE}; }

  static constexpr auto red() -> Color { return rgb(MAX_VALUE, 0, 0); }
  static constexpr auto green() -> Color { return rgb(0, MAX_VALUE, 0); }
  static constexpr auto blue() -> Color { return rgb(0, 0, MAX_VALUE); }
  static constexpr auto yellow() -> Color { return rgb(MAX_VALUE, MAX_VALUE, 0); }
  static constexpr auto purple() -> Color { return rgb(MAX_VALUE, 0, MAX_VALUE); }
  static constexpr auto cyan() -> Color { return rgb(0, MAX_VALUE, MAX_VALUE); }
  static constexpr auto white() -> Color { return rgb(MAX_VALUE, MAX_VALUE, MAX_VALUE); }
  static constexpr auto black() -> Color { return rgb(0, 0, 0); }
  static constexpr auto gray() -> Color { return rgb(GRAY_VALUE, GRAY_VALUE, GRAY_VALUE); }
  static constexpr auto darkGray() -> Color { return rgb(DARK_GRAY_VALUE, DARK_GRAY_VALUE, DARK_GRAY_VALUE); }
  static constexpr auto clear() -> Color { return rgba(0, 0, 0, 0); }

  [[nodiscard]] constexpr auto r() const -> u8 { return r_; }
  [[nodiscard]] constexpr auto g() const -> u8 { return g_; }
  [[nodiscard]] constexpr auto b() const -> u8 { return b_; }
  [[nodiscard]] constexpr auto a() const -> u8 { return a_; }

  [[nodiscard]] constexpr auto rRef() -> u8& { return r_; }
  [[nodiscard]] constexpr auto gRef() -> u8& { return g_; }
  [[nodiscard]] constexpr auto bRef() -> u8& { return b_; }
  [[nodiscard]] constexpr auto aRef() -> u8& { return a_; }

private:
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  constexpr Color(u8 red, u8 green, u8 blue, u8 alpha)
      : r_(red)
      , g_(green)
      , b_(blue)
      , a_(alpha)
  {
  }

  u8 r_ = MAX_VALUE;
  u8 g_ = MAX_VALUE;
  u8 b_ = MAX_VALUE;
  u8 a_ = MAX_VALUE;
};

struct Rect {
  Rect() = default;
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Rect(const Vec2& pos, const Vec2& size)
      : position_(pos)
      , size_(size)
  {
  }

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Rect(f32 xPos, f32 yPos, f32 width, f32 height)
      : position_(xPos, yPos)
      , size_(width, height)
  {
  }

  [[nodiscard]] auto left() const -> f32 { return position_.x; }
  [[nodiscard]] auto right() const -> f32 { return position_.x + size_.x; }
  [[nodiscard]] auto top() const -> f32 { return position_.y; }
  [[nodiscard]] auto bottom() const -> f32 { return position_.y + size_.y; }
  [[nodiscard]] auto center() const -> Vec2 { return position_ + (size_ * HALF_F); }

  [[nodiscard]] constexpr auto position() const -> Vec2 { return position_; }
  [[nodiscard]] constexpr auto size() const -> Vec2 { return size_; }

private:
  Vec2 position_{ZERO_F, ZERO_F};
  Vec2 size_{ONE_F, ONE_F};
};

struct Circle {
  Circle() = default;
  Circle(const Vec2& center, f32 radius)
      : center_(center)
      , radius_(radius)
  {
  }

  [[nodiscard]] constexpr auto center() const -> Vec2 { return center_; }
  [[nodiscard]] constexpr auto radius() const -> f32 { return radius_; }

private:
  Vec2 center_{ZERO_F, ZERO_F};
  f32 radius_ = ONE_F;
};

struct Line {
  Line() = default;

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  Line(const Vec2& start, const Vec2& end, f32 thickness = ONE_F)
      : start_(start)
      , end_(end)
      , thickness_(thickness)
  {
  }
  [[nodiscard]] constexpr auto start() const -> Vec2 { return start_; }
  [[nodiscard]] constexpr auto end() const -> Vec2 { return end_; }
  [[nodiscard]] constexpr auto thickness() const -> f32 { return thickness_; }

private:
  Vec2 start_{ZERO_F, ZERO_F};
  Vec2 end_{ONE_F, ONE_F};
  f32 thickness_ = ONE_F;
};

using TextureId = void*;

EGE_NAMESPACE_END