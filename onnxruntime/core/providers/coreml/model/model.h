// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "core/common/status.h"

namespace onnxruntime {
namespace coreml {

class Execution;

struct OnnxTensorData {
  const int32_t data_type;  // Uses TensorProto::DataType
  const std::vector<int64_t> shape;
  void* buffer{nullptr};
};

class Model {
 public:
  Model(const std::string& path);
  ~Model();
  Model(const Model&) = delete;
  Model& operator=(const Model&) = delete;

  onnxruntime::common::Status LoadModel();
  onnxruntime::common::Status Predict(const std::unordered_map<std::string, OnnxTensorData>& inputs,
                                      const std::unordered_map<std::string, OnnxTensorData>& outputs);

 private:
  std::unique_ptr<Execution> execution_;
};

}  // namespace coreml
}  // namespace onnxruntime