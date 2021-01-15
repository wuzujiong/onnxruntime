// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <core/graph/graph_viewer.h>
#include "coreml/Model.pb.h"

// namespace CoreML {
// namespace Specification {
// class Model;
// }
// }  // namespace CoreML

namespace onnxruntime {

namespace coreml {

class ModelBuilder {
 public:
  ModelBuilder(const GraphViewer& graph_viewer);
  ~ModelBuilder() = default;

  Status Compile(std::unique_ptr<Model>& model) ORT_MUST_USE_RESULT;

  // Accessors for members
  const GraphViewer& GetGraphViewer() const { return graph_viewer_; }

 private:
  const GraphViewer& graph_viewer_;
  std::unique_ptr<CoreML::Specification::Model> coreml_model_;

  // Convert the onnx model to CoreML::Specification::Model
  Status Prepare() ORT_MUST_USE_RESULT;

  // If a CoreML operation will use initializers directly, we will add the initializers to the skip list
  void PreprocessInitializers();

  // Copy and process all the initializers to CoreML model
  Status RegisterInitializers() ORT_MUST_USE_RESULT;

  Status RegisterModelInputs() ORT_MUST_USE_RESULT;
  Status AddOperations() ORT_MUST_USE_RESULT;
  Status RegisterModelOutputs() ORT_MUST_USE_RESULT;
};

}  // namespace coreml
}  // namespace onnxruntime
