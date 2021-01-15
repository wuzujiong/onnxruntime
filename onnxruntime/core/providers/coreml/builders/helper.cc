// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "helper.h"
#include <core/graph/graph.h>

namespace onnxruntime {
namespace coreml {

// TODO, move this to shared_library
bool GetType(const NodeArg& node_arg, int32_t& type) {
  type = ONNX_NAMESPACE::TensorProto_DataType_UNDEFINED;
  const auto* type_proto = node_arg.TypeAsProto();
  if (!type_proto || !type_proto->has_tensor_type() || !type_proto->tensor_type().has_elem_type()) {
    LOGS_DEFAULT(WARNING) << "NodeArg [" << node_arg.Name() << "] has no input type";
    return false;
  }

  type = type_proto->tensor_type().elem_type();
  return true;
}

}  // namespace coreml
}  // namespace onnxruntime