#pragma once

/**
 * @file    components/name.hpp
 * @brief   Name component for entity identification
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/ecs/component.hpp"

EGE_NAMESPACE_BEGIN

struct NameData {
  String m_name;
};

class Name : public Component<Name>
{
public:
  Name() = default;
  Name(String name)
      : m_data{std::move(name)}
  {
  }

  [[nodiscard]] auto name() const -> const String& { return m_data.m_name; }
  [[nodiscard]] auto data() const -> const NameData& { return m_data; }

  void setName(const String& name) { m_data.m_name = name; }

  [[nodiscard]] auto toString() const -> String override { return fmt::format("Name(\"{}\")", m_data.m_name); }

  [[nodiscard]] auto explain() const -> String override
  {
    return fmt::format("Name Component:\n"
                       "└─ Name: \"{}\" - human-readable identifier for entity",
                       m_data.m_name);
  }

private:
  NameData m_data;
};

EGE_NAMESPACE_END