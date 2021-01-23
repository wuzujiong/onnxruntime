// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <unordered_map>
#include "core/common/status.h"
#include "core/graph/basic_types.h"

namespace CoreML {
namespace Specification {
class WeightParams;
}
}  // namespace CoreML

namespace onnxruntime {
namespace coreml {

common::Status CreateCoreMLWeight(CoreML::Specification::WeightParams& weight,
                                  const ONNX_NAMESPACE::TensorProto& tensor);

}  // namespace coreml
}  // namespace onnxruntime
