/**
 * Title:        MaxSizeCodec.cpp
 * Description:  Max Size Codec
 * Copyright (c) 2020 Software AG, Darmstadt, Germany andor its licensors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the License at
 * http:/www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#define __STDC_FORMAT_MACROS 1
#include <sag_connectivity_plugins.hpp>
#include <inttypes.h>

using namespace com::softwareag::connectivity;

namespace com {
namespace apamax {

/**
 * This codec enforces a maximum payload size on a binary payload and 
 * drops any larger messages with an error.
 */
class MaxSizeCodec: public AbstractSimpleCodec
{
public:
	/// Parse the config file at startup
	MaxSizeCodec(const CodecConstructorParameters &params)
		: AbstractSimpleCodec(params),
		maxSize(0)
	{
		MapExtractor configEx(config, "config");
		int64_t s = configEx.get<int64_t>(data_t("maxSizeBytes"));
		if (s < 0) {
			throw std::runtime_error("maxSizeBytes must be 0 (infinite) or a positive integer");
		}
		maxSize = uint64_t(s);
		configEx.checkNoItemsRemaining();
	}

	 bool transformMessageTowardsHost(Message &msg) override
	 {
		 return true; // only filter on messages going to the transport
	 }

	 bool transformMessageTowardsTransport(Message &msg) override
	 {
		 if (maxSize >= 0 
				 && msg.getPayload().type_tag() == SAG_DATA_BUFFER
				 && get<buffer_t>(msg.getPayload()).size() > maxSize) {
			 logger.error("Dropping message because it exceeds the maximum size of %" PRIu64 " bytes (message was %" PRIu64 " bytes)", maxSize, uint64_t(get<buffer_t>(msg.getPayload()).size()));
			 return false;
		 } else {
			 return true; // skip the check if maxSize == 0
		 }
	 }

	 uint64_t maxSize;

};
/// Export this codec
SAG_DECLARE_CONNECTIVITY_CODEC_CLASS(MaxSizeCodec);

}}
