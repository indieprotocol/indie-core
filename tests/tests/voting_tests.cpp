/*
 * Copyright (c) 2018 oxarbitrage, and contributors.
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

#include <boost/test/unit_test.hpp>

#include <graphene/app/database_api.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>

#include <iostream>

#include "../common/database_fixture.hpp"

using namespace graphene::chain;
using namespace graphene::chain::test;


BOOST_FIXTURE_TEST_SUITE(voting_tests, database_fixture)

BOOST_FIXTURE_TEST_CASE( committee_account_initialization_test, database_fixture )
{ try {
   // Check current default committee
   // By default chain is configured with INITIAL_COMMITTEE_MEMBER_COUNT=9 members
   const auto &committee_members = db.get_global_properties().active_committee_members;
   const auto &committee = committee_account(db);

   BOOST_CHECK_EQUAL(committee_members.size(), INITIAL_COMMITTEE_MEMBER_COUNT);
   BOOST_CHECK_EQUAL(committee.active.num_auths(), INITIAL_COMMITTEE_MEMBER_COUNT);

   generate_blocks(HARDFORK_533_TIME);
   generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);
   generate_block();
   set_expiration(db, trx);

   // Check that committee not changed after 533 hardfork
   // vote counting method changed, but any votes are absent
   const auto &committee_members_after_hf533 = db.get_global_properties().active_committee_members;
   const auto &committee_after_hf533 = committee_account(db);
   BOOST_CHECK_EQUAL(committee_members_after_hf533.size(), INITIAL_COMMITTEE_MEMBER_COUNT);
   BOOST_CHECK_EQUAL(committee_after_hf533.active.num_auths(), INITIAL_COMMITTEE_MEMBER_COUNT);

   // You can't use uninitialized committee after 533 hardfork
   // when any user with stake created (create_account method automatically set up votes for committee)
   // committee is incomplete and consist of random active members
   ACTOR(alice);
   fund(alice);
   generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);

   const auto &committee_after_hf533_with_stake = committee_account(db);
   BOOST_CHECK_LT(committee_after_hf533_with_stake.active.num_auths(), INITIAL_COMMITTEE_MEMBER_COUNT);

   // Initialize committee by voting for each memeber and for desired count
   vote_for_committee_and_witnesses(INITIAL_COMMITTEE_MEMBER_COUNT, INITIAL_WITNESS_COUNT);
   generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);

   const auto &committee_members_after_hf533_and_init = db.get_global_properties().active_committee_members;
   const auto &committee_after_hf533_and_init = committee_account(db);
   BOOST_CHECK_EQUAL(committee_members_after_hf533_and_init.size(), INITIAL_COMMITTEE_MEMBER_COUNT);
   BOOST_CHECK_EQUAL(committee_after_hf533_and_init.active.num_auths(), INITIAL_COMMITTEE_MEMBER_COUNT);

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE(put_my_witnesses)
{
   try
   {
      ACTORS( (witness0)
              (witness1)
              (witness2)
              (witness3)
              (witness4)
              (witness5)
              (witness6)
              (witness7)
              (witness8)
              (witness9)
              (witness10)
              (witness11)
              (witness12)
              (witness13) );

      // Upgrade all accounts to LTM
      upgrade_to_lifetime_member(witness0_id);
      upgrade_to_lifetime_member(witness1_id);
      upgrade_to_lifetime_member(witness2_id);
      upgrade_to_lifetime_member(witness3_id);
      upgrade_to_lifetime_member(witness4_id);
      upgrade_to_lifetime_member(witness5_id);
      upgrade_to_lifetime_member(witness6_id);
      upgrade_to_lifetime_member(witness7_id);
      upgrade_to_lifetime_member(witness8_id);
      upgrade_to_lifetime_member(witness9_id);
      upgrade_to_lifetime_member(witness10_id);
      upgrade_to_lifetime_member(witness11_id);
      upgrade_to_lifetime_member(witness12_id);
      upgrade_to_lifetime_member(witness13_id);

      // Create all the witnesses
      const witness_id_type witness0_witness_id = create_witness(witness0_id, witness0_private_key).get_id();
      const witness_id_type witness1_witness_id = create_witness(witness1_id, witness1_private_key).get_id();
      const witness_id_type witness2_witness_id = create_witness(witness2_id, witness2_private_key).get_id();
      const witness_id_type witness3_witness_id = create_witness(witness3_id, witness3_private_key).get_id();
      const witness_id_type witness4_witness_id = create_witness(witness4_id, witness4_private_key).get_id();
      const witness_id_type witness5_witness_id = create_witness(witness5_id, witness5_private_key).get_id();
      const witness_id_type witness6_witness_id = create_witness(witness6_id, witness6_private_key).get_id();
      const witness_id_type witness7_witness_id = create_witness(witness7_id, witness7_private_key).get_id();
      const witness_id_type witness8_witness_id = create_witness(witness8_id, witness8_private_key).get_id();
      const witness_id_type witness9_witness_id = create_witness(witness9_id, witness9_private_key).get_id();
      const witness_id_type witness10_witness_id = create_witness(witness10_id, witness10_private_key).get_id();
      const witness_id_type witness11_witness_id = create_witness(witness11_id, witness11_private_key).get_id();
      const witness_id_type witness12_witness_id = create_witness(witness12_id, witness12_private_key).get_id();
      const witness_id_type witness13_witness_id = create_witness(witness13_id, witness13_private_key).get_id();

      // Create a vector with private key of all witnesses, will be used to activate 9 witnesses at a time
      const vector <fc::ecc::private_key> private_keys = {
            witness0_private_key,
            witness1_private_key,
            witness2_private_key,
            witness3_private_key,
            witness4_private_key,
            witness5_private_key,
            witness6_private_key,
            witness7_private_key,
            witness8_private_key,
            witness9_private_key,
            witness10_private_key,
            witness11_private_key,
            witness12_private_key,
            witness13_private_key

      };

      // create a map with account id and witness id
      const flat_map <account_id_type, witness_id_type> witness_map = {
            {witness0_id, witness0_witness_id},
            {witness1_id, witness1_witness_id},
            {witness2_id, witness2_witness_id},
            {witness3_id, witness3_witness_id},
            {witness4_id, witness4_witness_id},
            {witness5_id, witness5_witness_id},
            {witness6_id, witness6_witness_id},
            {witness7_id, witness7_witness_id},
            {witness8_id, witness8_witness_id},
            {witness9_id, witness9_witness_id},
            {witness10_id, witness10_witness_id},
            {witness11_id, witness11_witness_id},
            {witness12_id, witness12_witness_id},
            {witness13_id, witness13_witness_id}
      };

      // Check current default witnesses, default chain is configured with 9 witnesses
      auto witnesses = db.get_global_properties().active_witnesses;
      BOOST_CHECK_EQUAL(witnesses.size(), INITIAL_WITNESS_COUNT);
      BOOST_CHECK_EQUAL(witnesses.begin()[0].instance.value, 1u);
      BOOST_CHECK_EQUAL(witnesses.begin()[1].instance.value, 2u);
      BOOST_CHECK_EQUAL(witnesses.begin()[2].instance.value, 3u);
      BOOST_CHECK_EQUAL(witnesses.begin()[3].instance.value, 4u);
      BOOST_CHECK_EQUAL(witnesses.begin()[4].instance.value, 5u);
      BOOST_CHECK_EQUAL(witnesses.begin()[5].instance.value, 6u);
      BOOST_CHECK_EQUAL(witnesses.begin()[6].instance.value, 7u);
      BOOST_CHECK_EQUAL(witnesses.begin()[7].instance.value, 8u);
      BOOST_CHECK_EQUAL(witnesses.begin()[8].instance.value, 9u);

      // Activate all witnesses
      // Each witness is voted with incremental stake so last witness created will be the ones with more votes
      int c = 0;
      for (auto l : witness_map) {
         int stake = 100 + c + 10;
         transfer(committee_account, l.first, asset(stake));
         {
            set_expiration(db, trx);
            account_update_operation op;
            op.account = l.first;
            op.new_options = l.first(db).options;
            op.new_options->votes.insert(l.second(db).vote_id);

            trx.operations.push_back(op);
            sign(trx, private_keys.at(c));
            PUSH_TX(db, trx);
            trx.clear();
         }
         ++c;
      }

      // Trigger the new witnesses
      generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);
      generate_block();

      // Check my witnesses are now in control of the system
      witnesses = db.get_global_properties().active_witnesses;
      BOOST_CHECK_EQUAL(witnesses.size(), INITIAL_WITNESS_COUNT);
      BOOST_CHECK_EQUAL(witnesses.begin()[0].instance.value, 16u);
      BOOST_CHECK_EQUAL(witnesses.begin()[1].instance.value, 17u);
      BOOST_CHECK_EQUAL(witnesses.begin()[2].instance.value, 18u);
      BOOST_CHECK_EQUAL(witnesses.begin()[3].instance.value, 19u);
      BOOST_CHECK_EQUAL(witnesses.begin()[4].instance.value, 20u);
      BOOST_CHECK_EQUAL(witnesses.begin()[5].instance.value, 21u);
      BOOST_CHECK_EQUAL(witnesses.begin()[6].instance.value, 22u);
      BOOST_CHECK_EQUAL(witnesses.begin()[7].instance.value, 23u);
      BOOST_CHECK_EQUAL(witnesses.begin()[8].instance.value, 24u);

   } FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE(track_votes_witnesses_enabled)
{
   try
   {
      graphene::app::database_api db_api1(db);

      INVOKE(put_my_witnesses);

      const account_id_type witness1_id= get_account("witness1").get_id();
      auto witness1_object = db_api1.get_witness_by_account(witness1_id(db).name);
      BOOST_CHECK_EQUAL(witness1_object->total_votes, 111u);

   } FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE(track_votes_witnesses_disabled)
{
   try
   {
      graphene::app::database_api db_api1(db);

      INVOKE(put_my_witnesses);

      const account_id_type witness1_id= get_account("witness1").get_id();
      auto witness1_object = db_api1.get_witness_by_account(witness1_id(db).name);
      BOOST_CHECK_EQUAL(witness1_object->total_votes, 0u);

   } FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE(put_my_committee_members)
{
   try
   {
      ACTORS( (committee0)
              (committee1)
              (committee2)
              (committee3)
              (committee4)
              (committee5)
              (committee6)
              (committee7)
              (committee8)
              (committee9)
              (committee10)
              (committee11)
              (committee12)
              (committee13) );

      // Upgrade all accounts to LTM
      upgrade_to_lifetime_member(committee0_id);
      upgrade_to_lifetime_member(committee1_id);
      upgrade_to_lifetime_member(committee2_id);
      upgrade_to_lifetime_member(committee3_id);
      upgrade_to_lifetime_member(committee4_id);
      upgrade_to_lifetime_member(committee5_id);
      upgrade_to_lifetime_member(committee6_id);
      upgrade_to_lifetime_member(committee7_id);
      upgrade_to_lifetime_member(committee8_id);
      upgrade_to_lifetime_member(committee9_id);
      upgrade_to_lifetime_member(committee10_id);
      upgrade_to_lifetime_member(committee11_id);
      upgrade_to_lifetime_member(committee12_id);
      upgrade_to_lifetime_member(committee13_id);

      // Create all the committee
      const committee_member_id_type committee0_committee_id = create_committee_member(committee0_id(db)).get_id();
      const committee_member_id_type committee1_committee_id = create_committee_member(committee1_id(db)).get_id();
      const committee_member_id_type committee2_committee_id = create_committee_member(committee2_id(db)).get_id();
      const committee_member_id_type committee3_committee_id = create_committee_member(committee3_id(db)).get_id();
      const committee_member_id_type committee4_committee_id = create_committee_member(committee4_id(db)).get_id();
      const committee_member_id_type committee5_committee_id = create_committee_member(committee5_id(db)).get_id();
      const committee_member_id_type committee6_committee_id = create_committee_member(committee6_id(db)).get_id();
      const committee_member_id_type committee7_committee_id = create_committee_member(committee7_id(db)).get_id();
      const committee_member_id_type committee8_committee_id = create_committee_member(committee8_id(db)).get_id();
      const committee_member_id_type committee9_committee_id = create_committee_member(committee9_id(db)).get_id();
      const committee_member_id_type committee10_committee_id = create_committee_member(committee10_id(db)).get_id();
      const committee_member_id_type committee11_committee_id = create_committee_member(committee11_id(db)).get_id();
      const committee_member_id_type committee12_committee_id = create_committee_member(committee12_id(db)).get_id();
      const committee_member_id_type committee13_committee_id = create_committee_member(committee13_id(db)).get_id();

      // Create a vector with private key of all committee members, will be used to activate 9 members at a time
      const vector <fc::ecc::private_key> private_keys = {
            committee0_private_key,
            committee1_private_key,
            committee2_private_key,
            committee3_private_key,
            committee4_private_key,
            committee5_private_key,
            committee6_private_key,
            committee7_private_key,
            committee8_private_key,
            committee9_private_key,
            committee10_private_key,
            committee11_private_key,
            committee12_private_key,
            committee13_private_key
      };

      // create a map with account id and committee member id
      const flat_map <account_id_type, committee_member_id_type> committee_map = {
            {committee0_id, committee0_committee_id},
            {committee1_id, committee1_committee_id},
            {committee2_id, committee2_committee_id},
            {committee3_id, committee3_committee_id},
            {committee4_id, committee4_committee_id},
            {committee5_id, committee5_committee_id},
            {committee6_id, committee6_committee_id},
            {committee7_id, committee7_committee_id},
            {committee8_id, committee8_committee_id},
            {committee9_id, committee9_committee_id},
            {committee10_id, committee10_committee_id},
            {committee11_id, committee11_committee_id},
            {committee12_id, committee12_committee_id},
            {committee13_id, committee13_committee_id}
      };

      // Check current default committee, default chain is configured with 9 committee members
      auto committee_members = db.get_global_properties().active_committee_members;

      BOOST_CHECK_EQUAL(committee_members.size(), INITIAL_COMMITTEE_MEMBER_COUNT);
      BOOST_CHECK_EQUAL(committee_members.begin()[0].instance.value, 0u);
      BOOST_CHECK_EQUAL(committee_members.begin()[1].instance.value, 1u);
      BOOST_CHECK_EQUAL(committee_members.begin()[2].instance.value, 2u);
      BOOST_CHECK_EQUAL(committee_members.begin()[3].instance.value, 3u);
      BOOST_CHECK_EQUAL(committee_members.begin()[4].instance.value, 4u);
      BOOST_CHECK_EQUAL(committee_members.begin()[5].instance.value, 5u);
      BOOST_CHECK_EQUAL(committee_members.begin()[6].instance.value, 6u);
      BOOST_CHECK_EQUAL(committee_members.begin()[7].instance.value, 7u);
      BOOST_CHECK_EQUAL(committee_members.begin()[8].instance.value, 8u);

      // Activate all committee
      // Each committee is voted with incremental stake so last member created will be the ones with more votes
      int c = 0;
      for (auto committee : committee_map) {
         int stake = 100 + c + 10;
         transfer(committee_account, committee.first, asset(stake));
         {
            set_expiration(db, trx);
            account_update_operation op;
            op.account = committee.first;
            op.new_options = committee.first(db).options;

            op.new_options->votes.clear();
            op.new_options->votes.insert(committee.second(db).vote_id);
            op.new_options->num_committee = 1;

            trx.operations.push_back(op);
            sign(trx, private_keys.at(c));
            PUSH_TX(db, trx);
            trx.clear();
         }
         ++c;
      }

      // Trigger the new committee
      generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);
      generate_block();

      // Check my witnesses are now in control of the system
      committee_members = db.get_global_properties().active_committee_members;
      std::sort(committee_members.begin(), committee_members.end());

      BOOST_CHECK_EQUAL(committee_members.size(), INITIAL_COMMITTEE_MEMBER_COUNT);

      // Check my committee members are now in control of the system
      BOOST_CHECK_EQUAL(committee_members.begin()[0].instance.value, 15);
      BOOST_CHECK_EQUAL(committee_members.begin()[1].instance.value, 16);
      BOOST_CHECK_EQUAL(committee_members.begin()[2].instance.value, 17);
      BOOST_CHECK_EQUAL(committee_members.begin()[3].instance.value, 18);
      BOOST_CHECK_EQUAL(committee_members.begin()[4].instance.value, 19);
      BOOST_CHECK_EQUAL(committee_members.begin()[5].instance.value, 20);
      BOOST_CHECK_EQUAL(committee_members.begin()[6].instance.value, 21);
      BOOST_CHECK_EQUAL(committee_members.begin()[7].instance.value, 22);
      BOOST_CHECK_EQUAL(committee_members.begin()[8].instance.value, 23);

   } FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE(track_votes_committee_enabled)
{
   try
   {
      graphene::app::database_api db_api1(db);

      INVOKE(put_my_committee_members);

      const account_id_type committee1_id= get_account("committee1").get_id();
      auto committee1_object = db_api1.get_committee_member_by_account(committee1_id(db).name);
      BOOST_CHECK_EQUAL(committee1_object->total_votes, 111u);

   } FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE(track_votes_committee_disabled)
{
   try
   {
      graphene::app::database_api db_api1(db);

      INVOKE(put_my_committee_members);

      const account_id_type committee1_id= get_account("committee1").get_id();
      auto committee1_object = db_api1.get_committee_member_by_account(committee1_id(db).name);
      BOOST_CHECK_EQUAL(committee1_object->total_votes, 0u);

   } FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE(invalid_voting_account)
{
   try
   {
      ACTORS((alice));

      account_id_type invalid_account_id( (uint64_t)999999 );

      BOOST_CHECK( !db.find( invalid_account_id ) );

      graphene::chain::account_update_operation op;
      op.account = alice_id;
      op.new_options = alice.options;
      op.new_options->voting_account = invalid_account_id;
      trx.operations.push_back(op);
      sign(trx, alice_private_key);

      GRAPHENE_REQUIRE_THROW( PUSH_TX( db, trx, ~0 ), fc::exception );

   } FC_LOG_AND_RETHROW()
}
BOOST_AUTO_TEST_CASE(last_voting_date)
{
   try
   {
      ACTORS((alice));

      transfer(committee_account, alice_id, asset(100));

      // we are going to vote for this witness
      auto witness1 = witness_id_type(1)(db);

      auto stats_obj = db.get_account_stats_by_owner(alice_id);
      BOOST_CHECK_EQUAL(stats_obj.last_vote_time.sec_since_epoch(), 0u);

      // alice votes
      graphene::chain::account_update_operation op;
      op.account = alice_id;
      op.new_options = alice.options;
      op.new_options->votes.insert(witness1.vote_id);
      trx.operations.push_back(op);
      sign(trx, alice_private_key);
      PUSH_TX( db, trx, ~0 );

      auto now = db.head_block_time().sec_since_epoch();

      // last_vote_time is updated for alice
      stats_obj = db.get_account_stats_by_owner(alice_id);
      BOOST_CHECK_EQUAL(stats_obj.last_vote_time.sec_since_epoch(), now);

   } FC_LOG_AND_RETHROW()
}
BOOST_AUTO_TEST_CASE(last_voting_date_proxy)
{
   try
   {
      ACTORS((alice)(proxy)(bob));

      transfer(committee_account, alice_id, asset(100));
      transfer(committee_account, bob_id, asset(200));
      transfer(committee_account, proxy_id, asset(300));

      generate_block();

      // witness to vote for
      auto witness1 = witness_id_type(1)(db);

      // round1: alice changes proxy, this is voting activity
      {
         graphene::chain::account_update_operation op;
         op.account = alice_id;
         op.new_options = alice_id(db).options;
         op.new_options->voting_account = proxy_id;
         trx.operations.push_back(op);
         sign(trx, alice_private_key);
         PUSH_TX( db, trx, ~0 );
      }
      // alice last_vote_time is updated
      auto alice_stats_obj = db.get_account_stats_by_owner(alice_id);
      auto round1 = db.head_block_time().sec_since_epoch();
      BOOST_CHECK_EQUAL(alice_stats_obj.last_vote_time.sec_since_epoch(), round1);

      generate_block();

      // round 2: alice update account but no proxy or voting changes are done
      {
         graphene::chain::account_update_operation op;
         op.account = alice_id;
         op.new_options = alice_id(db).options;
         trx.operations.push_back(op);
         sign(trx, alice_private_key);
         set_expiration( db, trx );
         PUSH_TX( db, trx, ~0 );
      }
      // last_vote_time is not updated
      alice_stats_obj = db.get_account_stats_by_owner(alice_id);
      BOOST_CHECK_EQUAL(alice_stats_obj.last_vote_time.sec_since_epoch(), round1);

      generate_block();

      // round 3: bob votes
      {
         graphene::chain::account_update_operation op;
         op.account = bob_id;
         op.new_options = bob_id(db).options;
         op.new_options->votes.insert(witness1.vote_id);
         trx.operations.push_back(op);
         sign(trx, bob_private_key);
         set_expiration( db, trx );
         PUSH_TX(db, trx, ~0);
      }

      // last_vote_time for bob is updated as he voted
      auto round3 = db.head_block_time().sec_since_epoch();
      auto bob_stats_obj = db.get_account_stats_by_owner(bob_id);
      BOOST_CHECK_EQUAL(bob_stats_obj.last_vote_time.sec_since_epoch(), round3);

      generate_block();

      // round 4: proxy votes
      {
         graphene::chain::account_update_operation op;
         op.account = proxy_id;
         op.new_options = proxy_id(db).options;
         op.new_options->votes.insert(witness1.vote_id);
         trx.operations.push_back(op);
         sign(trx, proxy_private_key);
         PUSH_TX(db, trx, ~0);
      }

      // proxy just voted so the last_vote_time is updated
      auto round4 = db.head_block_time().sec_since_epoch();
      auto proxy_stats_obj = db.get_account_stats_by_owner(proxy_id);
      BOOST_CHECK_EQUAL(proxy_stats_obj.last_vote_time.sec_since_epoch(), round4);

      // alice haves proxy, proxy votes but last_vote_time is not updated for alice
      alice_stats_obj = db.get_account_stats_by_owner(alice_id);
      BOOST_CHECK_EQUAL(alice_stats_obj.last_vote_time.sec_since_epoch(), round1);

      // bob haves nothing to do with proxy so last_vote_time is not updated
      bob_stats_obj = db.get_account_stats_by_owner(bob_id);
      BOOST_CHECK_EQUAL(bob_stats_obj.last_vote_time.sec_since_epoch(), round3);

   } FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_SUITE_END()
