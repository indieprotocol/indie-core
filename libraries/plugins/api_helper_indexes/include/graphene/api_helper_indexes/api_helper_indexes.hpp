/*
 * Copyright (c) 2018 api_helper_indexes and contributors.
 *
 * The MIT License
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
#pragma once

#include <graphene/app/plugin.hpp>
#include <graphene/protocol/types.hpp>

namespace graphene { namespace api_helper_indexes {
using namespace chain;

/**
 *  @brief This secondary index tracks how much of each asset is locked up as collateral for MPAs, and how much
 *         collateral is backing an MPA in total.
 */
class amount_in_collateral_index : public secondary_index
{
   public:
      virtual void object_inserted( const object& obj ) override;
      virtual void object_removed( const object& obj ) override;
      virtual void about_to_modify( const object& before ) override;
      virtual void object_modified( const object& after ) override;

      share_type get_amount_in_collateral( const asset_id_type& asset )const;
      share_type get_backing_collateral( const asset_id_type& asset )const;

   private:
      flat_map<asset_id_type, share_type> in_collateral;
      flat_map<asset_id_type, share_type> backing_collateral;
};

/**
 *  @brief This secondary index tracks the next ID of all object types.
 *  @note This is implemented with \c flat_map considering there aren't too many object types in the system thus
 *        the performance would be acceptable.
 */
class next_object_ids_index : public secondary_index
{
public:
   object_id_type get_next_id(uint8_t space_id, uint8_t type_id) const;

private:
   friend class api_helper_indexes;
   flat_map<std::pair<uint8_t, uint8_t>, object_id_type> _next_ids;
};

namespace detail
{
    class api_helper_indexes_impl;
}

class api_helper_indexes : public graphene::app::plugin
{
   public:
      explicit api_helper_indexes(graphene::app::application &app);
      ~api_helper_indexes() override;

      std::string plugin_name()const override;
      std::string plugin_description()const override;
      virtual void plugin_set_program_options(
         boost::program_options::options_description& cli,
         boost::program_options::options_description& cfg) override;
      virtual void plugin_initialize(const boost::program_options::variables_map& options) override;
      virtual void plugin_startup() override;

      friend class detail::api_helper_indexes_impl;

   private:
      std::unique_ptr<detail::api_helper_indexes_impl> my;
      amount_in_collateral_index* amount_in_collateral = nullptr;
      next_object_ids_index *next_object_ids_idx = nullptr;

      bool _next_ids_map_initialized = false;
      void refresh_next_ids();
};

} } //graphene::template
