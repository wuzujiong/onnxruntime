// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "coreml/Model.pb.h"

// namespace CoreML {
// namespace Specification {
// class Model;
// }
// }  // namespace CoreML

namespace onnxruntime {

class GraphViewer;

namespace coreml {

class ModelBuilder {
 public:
  ModelBuilder(const GraphViewer& graph_viewer);
  ~ModelBuilder() = default;

 private:
  const GraphViewer& graph_viewer_;
  std::unique_ptr<CoreML::Specification::Model> coreml_model_;
};

}  // namespace coreml
}  // namespace onnxruntime
