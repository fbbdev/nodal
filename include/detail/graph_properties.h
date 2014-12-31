/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Fabio Massaioli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __NODAL_DETAIL_GRAPH_PROPERTIES_H__
#define __NODAL_DETAIL_GRAPH_PROPERTIES_H__

#include <boost/graph/properties.hpp>

namespace boost
{

template<typename Value, bool Const = false>
class attribute_property_map
{
public:
  using value_type = Value;
  using reference = value_type;
  using key_type = nodal::graph_node*;
  using category = read_write_property_map_tag;

  attribute_property_map(std::string const& attribute)
    : attribute(attribute)
    {}

  std::string attribute;
};

template<typename Value>
class attribute_property_map<Value, true>
{
public:
  using value_type = Value;
  using reference = value_type;
  using key_type = nodal::graph_node const*;
  using category = readable_property_map_tag;

  attribute_property_map(std::string const& attribute)
    : attribute(attribute)
    {}

  std::string attribute;
};

template<typename T, bool Const>
T get(attribute_property_map<T, Const> const& pmap,
      nodal::graph_node const* key)
{
  if (key->has_attribute(pmap.attribute))
    return boost::any_cast<T>(key->attribute(pmap.attribute));
  else
    return T();
}

template<typename T>
void put(attribute_property_map<T, false> const& pmap, nodal::graph_node* key,
         T const& val)
{
  key->attribute(pmap.attribute) = val;
}

template<typename PropertyTag>
struct property_tag_to_attribute
{
  static constexpr bool mapped = false;
  static constexpr char const* name = nullptr;
};

template<typename PropertyTag>
struct property_map<nodal::graph, PropertyTag>
{
  using mapped_attribute = property_tag_to_attribute<PropertyTag>;

  template<bool Const>
  class map :
    public attribute_property_map<typename mapped_attribute::type, Const>
  {
    using base = attribute_property_map<typename mapped_attribute::type, Const>;

  public:
    using typename base::value_type;
    using typename base::reference;
    using typename base::key_type;
    using typename base::category;

    map(nodal::graph const& g) : base(mapped_attribute::name) {}
  };

  using type = map<false>;
  using const_type = map<true>;
};

template<>
struct property_tag_to_attribute<vertex_name_t>
{
  static constexpr bool mapped = true;
  static constexpr char const* name = "name";
  using type = std::string;
};

template<>
struct property_tag_to_attribute<vertex_distance_t>
{
  static constexpr bool mapped = true;
  static constexpr char const* name = "distance";
  using type = double;
};

template<>
struct property_tag_to_attribute<vertex_color_t>
{
  static constexpr bool mapped = true;
  static constexpr char const* name = "color";
  using type = default_color_type;
};

template<>
struct property_tag_to_attribute<vertex_discover_time_t>
{
  static constexpr bool mapped = true;
  static constexpr char const* name = "discover_time";
  using type = std::size_t;
};

template<>
struct property_tag_to_attribute<vertex_finish_time_t>
{
  static constexpr bool mapped = true;
  static constexpr char const* name = "finish_time";
  using type = std::size_t;
};

template<>
struct property_map<nodal::graph, vertex_index_t>
{
  using iter_diff_type =
    std::iterator_traits<nodal::graph::node_iterator>::difference_type;

  class const_type : public put_get_helper<iter_diff_type, const_type>
  {
  public:
    using value_type = iter_diff_type;
    using reference = value_type;
    using key_type = nodal::graph_node*;
    using category = readable_property_map_tag;

    const_type(nodal::graph const& g) : g(&g) {}

    reference operator[](key_type const& node) const
    {
      return std::distance(g->nodes_begin(), g->find(node));
    }

  private:
    nodal::graph const* g;
  };

  using type = const_type;
};

template<>
struct property_map<nodal::graph, edge_index_t>
{
  using iter_diff_type =
    std::iterator_traits<nodal::graph::link_iterator>::difference_type;

  class const_type : public put_get_helper<iter_diff_type, const_type>
  {
  public:
    using value_type = iter_diff_type;
    using reference = value_type;
    using key_type = nodal::graph_link;
    using category = readable_property_map_tag;

    const_type(nodal::graph const& g) : g(&g) {}

    reference operator[](key_type const& link) const
    {
      return std::distance(g->links_begin(), g->find(link));
    }

  private:
    nodal::graph const* g;
  };

  using type = const_type;
};

template<>
struct property_map<nodal::graph, edge_weight_t>
{
  class const_type : public put_get_helper<double, const_type>
  {
  public:
    using value_type = double;
    using reference = value_type;
    using key_type = nodal::graph_link;
    using category = readable_property_map_tag;

    const_type(nodal::graph const& g) {}

    reference operator[](key_type const& link) const
    {
      return 1.0;
    }
  };

  using type = const_type;
};

template<typename PropertyTag>
inline
typename property_map<nodal::graph, PropertyTag>::type
get(PropertyTag p, nodal::graph& g)
{
  return typename property_map<nodal::graph, PropertyTag>::type(g);
}

template<typename PropertyTag>
inline
typename property_map<nodal::graph, PropertyTag>::const_type
get(PropertyTag, nodal::graph const& g)
{
  return typename property_map<nodal::graph, PropertyTag>::const_type(g);
}

template<typename PropertyTag, typename Key>
inline
typename property_map<nodal::graph, PropertyTag>::type::reference
get(PropertyTag p, nodal::graph& g, Key&& x)
{
  return get(get(p, g), std::forward<Key>(x));
}

template<typename PropertyTag, typename Key>
inline
typename property_map<nodal::graph, PropertyTag>::const_type::reference
get(PropertyTag p, nodal::graph const& g, Key&& x)
{
  return get(get(p, g), std::forward<Key>(x));
}

template<typename PropertyTag, typename Key, typename Value>
inline
void put(PropertyTag p, nodal::graph& g, Key&& x, Value&& v)
{
  put(get(p, g), std::forward<Key>(x), std::forward<Value>(v));
}

} /* namespace boost */

#endif /* __NODAL_DETAIL_GRAPH_PROPERTIES_H__ */
