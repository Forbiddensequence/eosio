#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("test1")]] test1 : public eosio::contract {

public:

  test1(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void change(name user, uint64_t variable, std::string name) {
    require_auth( user );

    address_index addresses( get_self(), get_first_receiver().value );

    auto iterator = addresses.find(user.value);

    check(iterator != addresses.end(), "Record does not exist");
    addresses.modify(iterator, user, [&]( auto& row ) {
        row.key = user;
        row.name=name;
        row.variable=variable;
      });
    
  }

  [[eosio::action]]
  void init(name user, uint64_t variable, std::string name) {
    require_auth( user );

    address_index addresses( get_self(), get_first_receiver().value );

    auto iterator = addresses.find(user.value);
    
    addresses.emplace( user, [&]( auto& row ) {
        row.key = user;
        row.name = name;
        row.variable = variable;
      });
    
  }

  [[eosio::action]]
  void settozero(name user) {
    require_auth(user);

    address_index addresses( get_self(), get_first_receiver().value);

    auto iterator = addresses.find(user.value);
    check(iterator != addresses.end(), "Record does not exist");
    addresses.modify(iterator, user, [&]( auto& row ) {
        row.key = row.key;
        row.name = row.name;
        row.variable = 0;
      });
  }

private:
  struct [[eosio::table]] person {
    name key;
    uint64_t variable;
    std::string name;
    uint64_t primary_key() const { return key.value; }
  };
  using address_index = eosio::multi_index<"people"_n, person>;
};
