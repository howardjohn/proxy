#include <string>

#include "address_metadata.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the echo2 filter. @see NamedNetworkFilterConfigFactory.
 */
class AddressMetadataConfigFactory : public NamedNetworkFilterConfigFactory {
public:
  Network::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message&,
                                                        FactoryContext&) override {
    return [](Network::FilterManager& filter_manager) -> void {
      filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new Filter::AddressMetadata()});
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new Envoy::ProtobufWkt::Struct()};
  }

  std::string name() const override { return "address_metadata"; }

  bool isTerminalFilterByProto(const Protobuf::Message&, FactoryContext&) override { return false; }
};

/**
 * Static registration for the echo2 filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<AddressMetadataConfigFactory, NamedNetworkFilterConfigFactory> registered_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
