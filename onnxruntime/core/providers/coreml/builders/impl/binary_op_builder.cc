// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <core/providers/common.h>

#include "../model_builder.h"
#include "../helper.h"

#include "base_op_builder.h"

namespace onnxruntime {
namespace coreml {

class BinaryOpBuilder : public BaseOpBuilder {
 private:
  int GetMinSupportedOpSet(const Node& node) const override;
  Status AddToModelBuilderImpl(ModelBuilder& model_builder, const Node& node) const override ORT_MUST_USE_RESULT;
};

int BinaryOpBuilder::GetMinSupportedOpSet(const Node& /* node */) const {
  // Add/Sub/Mul/Div opset 6- has broadcast attributes we do not support now
  return 7;
}

Status BinaryOpBuilder::AddToModelBuilderImpl(ModelBuilder& /* model_builder */, const Node& /* node */) const {
  return Status::OK();
}

std::unique_ptr<IOpBuilder> CreateBinaryOpBuilder() {
  return onnxruntime::make_unique<BinaryOpBuilder>();
}

}  // namespace coreml
}  // namespace onnxruntime
