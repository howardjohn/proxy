#include "address_metadata.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

#include "source/common/common/assert.h"

namespace Envoy {
    namespace Filter {

        Network::FilterStatus AddressMetadata::onData(Buffer::Instance& data, bool) {
            ENVOY_CONN_LOG(info, "echo: got {} bytes with address {}", read_callbacks_->connection(), data.length(), \
                           read_callbacks_->connection().streamInfo().downstreamAddressProvider().localAddress());

            envoy::config::core::v3::Metadata& dynamic_metadata =
                    read_callbacks_->connection().streamInfo().dynamicMetadata();

            ProtobufWkt::Struct metadata(
                    (*dynamic_metadata.mutable_filter_metadata())["envoy.lb"]);

            auto& fields = *metadata.mutable_fields();
            auto val = ProtobufWkt::Value();
            absl::string_view sv = read_callbacks_->connection().streamInfo().downstreamAddressProvider().localAddress()->asStringView();
            std::string s = {sv.begin(), sv.end()};
            val.set_string_value(s);

            fields.insert({"address", val});

            read_callbacks_->connection().streamInfo().setDynamicMetadata(
                    "envoy.lb", metadata);

            return Network::FilterStatus::Continue;
        }

    } // namespace Filter
} // namespace Envoy