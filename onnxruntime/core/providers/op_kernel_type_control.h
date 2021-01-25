// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <tuple>

#include "boost/mp11.hpp"

#include "core/common/type_list.h"

namespace onnxruntime {

namespace op_kernel_type_control {

enum class OpKernelArgDirection {
  Input,
  Output
};

using OpKernelArgIndex = size_t;

namespace tags {
// a tag that identifies the target (Op kernel argument) of the specified types
template <typename OpKernelTag, OpKernelArgDirection ArgDirection, OpKernelArgIndex ArgIndex>
struct OpKernelArg {};

// a tag that indicates the supported types for a particular Op kernel argument, identified by OpKernelArgTag
template <typename OpKernelArgTag>
struct Supported {};

// a tag that indicates the allowed types for a particular Op kernel argument, identified by OpKernelArgTag
template <typename OpKernelArgTag>
struct Allowed {};

// a tag that indicates the globally allowed types
struct GlobalAllowed {};
}  // namespace tags

// contains specified Op kernel argument types
// if types are defined (optional), the data member 'value' should be a type list
// see the tags in onnxruntime::op_kernel_type_control::tags for intended uses
template <typename Tag>
struct OpKernelArgTypes {};

// gives access to the enabled Op kernel argument types via the 'type' data member
template <typename OpKernelArgTag>
struct EnabledOpKernelArgTypes {
 private:
  template <typename T>
  using GetTypeMember = typename T::type;

  template <typename T>
  using HasTypeMember = boost::mp11::mp_valid<GetTypeMember, T>;

  static_assert(HasTypeMember<OpKernelArgTypes<tags::Supported<OpKernelArgTag>>>::value,
                "OpKernelArgTypes<Supported<OpKernelArgTag>> must have a 'type' data member. "
                "Check that the Op kernel argument supported types are specified.");

  using TypeMembers =
      boost::mp11::mp_transform<
          GetTypeMember,
          boost::mp11::mp_append<
              // OpKernelArgTypes<Supported<OpKernelArgTag>> should always contain a 'type' member
              TypeList<
                  OpKernelArgTypes<tags::Supported<OpKernelArgTag>>>,
              // OpKernelArgTypes<Reduced<OpKernelArgTag>> and OpKernelArgTypes<GlobalReduced> each may
              // optionally contain a 'type' member, so only include their 'type's if they do
              boost::mp11::mp_filter<
                  HasTypeMember,
                  TypeList<
                      OpKernelArgTypes<tags::Allowed<OpKernelArgTag>>,
                      OpKernelArgTypes<tags::GlobalAllowed>>>>>;

  static_assert(boost::mp11::mp_all_of<TypeMembers, boost::mp11::mp_is_list>::value,
                "All OpKernelArgTypes<Tag> 'type' data members must be type lists.");

  template <typename L>
  using MakeSet =
      boost::mp11::mp_apply<
          boost::mp11::mp_set_push_back,
          boost::mp11::mp_append<TypeList<TypeList<>>, L>>;

  using TypeMemberSets = boost::mp11::mp_transform<MakeSet, TypeMembers>;

 public:
  using type = boost::mp11::mp_apply<boost::mp11::mp_set_intersection, TypeMemberSets>;
};

}  // namespace op_kernel_type_control
}  // namespace onnxruntime

// INTERNAL
// the class name of a tag type identifying an Op kernel for the purposes of type control
#define ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_TAG_CLASS_NAME(OpKernel) \
  OpKernelTypeControlFor##OpKernel##Tag

// INTERNAL
// a tag type identifying an Op kernel argument
#define ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_KERNEL_ARG_TAG(OpKernel, ArgDirection, ArgIndex) \
  ::onnxruntime::op_kernel_type_control::tags::OpKernelArg<                                  \
      ::onnxruntime::op_kernel_type_control::                                                \
          ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_TAG_CLASS_NAME(OpKernel),                      \
      ::onnxruntime::op_kernel_type_control::OpKernelArgDirection::ArgDirection,             \
      ArgIndex>

// public macros

/**
 * Specifies the supported types for a given Op kernel argument.
 * This should be specified with the Op kernel implementation.
 *
 * Note: This should be called from the onnxruntime::op_kernel_type_control namespace.
 *
 * @param Op The Op name.
 * @param ArgDirection Direction of the given Op kernel argument - Input or Output.
 * @param ArgIndex Index of the given Op kernel argument.
 * @param ... The types.
 */
#define ORT_SPECIFY_OP_KERNEL_ARG_SUPPORTED_TYPES(Op, ArgDirection, ArgIndex, ...)           \
  class ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_TAG_CLASS_NAME(Op);                              \
  template <>                                                                                \
  struct ::onnxruntime::op_kernel_type_control::OpKernelArgTypes<                            \
      ::onnxruntime::op_kernel_type_control::tags::Supported<                                \
          ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_KERNEL_ARG_TAG(Op, ArgDirection, ArgIndex)>> { \
    using type = ::onnxruntime::TypeList<__VA_ARGS__>;                                       \
  };

/**
 * Specifies an allowed set of types for a given Op kernel argument.
 * This can optionally be specified to further limit the enabled types.
 *
 * Note: This should be called from the onnxruntime::op_kernel_type_control namespace.
 *
 * @param Op The Op name.
 * @param ArgDirection Direction of the given Op kernel argument - Input or Output.
 * @param ArgIndex Index of the given Op kernel argument.
 * @param ... The types.
 */
#define ORT_SPECIFY_OP_KERNEL_ARG_ALLOWED_TYPES(Op, ArgDirection, ArgIndex, ...)             \
  class ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_TAG_CLASS_NAME(Op);                              \
  template <>                                                                                \
  struct ::onnxruntime::op_kernel_type_control::OpKernelArgTypes<                            \
      ::onnxruntime::op_kernel_type_control::tags::Allowed<                                  \
          ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_KERNEL_ARG_TAG(Op, ArgDirection, ArgIndex)>> { \
    using type = ::onnxruntime::TypeList<__VA_ARGS__>;                                       \
  };

/**
 * Specifies an allowed set of types globally (applicable to any Op kernel argument).
 * This can optionally be specified to further limit the enabled types.
 * 
 * Note: This should be called from the onnxruntime::op_kernel_type_control namespace.
 * 
 * @param ... The types.
 */
#define ORT_SPECIFY_OP_KERNEL_GLOBAL_ALLOWED_TYPES(...)             \
  template <>                                                       \
  struct ::onnxruntime::op_kernel_type_control::OpKernelArgTypes<   \
      ::onnxruntime::op_kernel_type_control::tags::GlobalAllowed> { \
    using type = ::onnxruntime::TypeList<__VA_ARGS__>;              \
  };

/**
 * TypeList type with the enabled types for a given Op kernel argument.
 *
 * Note: At the call site, the onnxruntime::op_kernel_type_control namespace should be visible.
 *
 * @param Op The Op name.
 * @param ArgDirection Direction of the given Op kernel argument - Input or Output.
 * @param ArgIndex Index of the given Op kernel argument.
 */
#define ORT_OP_KERNEL_ARG_ENABLED_TYPE_LIST(Op, ArgDirection, ArgIndex) \
  ::onnxruntime::op_kernel_type_control::EnabledOpKernelArgTypes<       \
      ORT_OP_KERNEL_TYPE_CTRL_INTERNAL_OP_KERNEL_ARG_TAG(               \
          Op, ArgDirection, ArgIndex)>::type

/**
 * std::tuple type with the enabled types for a given Op kernel argument.
 *
 * Note: At the call site, the onnxruntime::op_kernel_type_control namespace should be visible.
 *
 * @param Op The Op name.
 * @param ArgDirection Direction of the given Op kernel argument - Input or Output.
 * @param ArgIndex Index of the given Op kernel argument.
 */
#define ORT_OP_KERNEL_ARG_ENABLED_TYPE_TUPLE(Op, ArgDirection, ArgIndex) \
  boost::mp11::mp_rename<                                                \
      ORT_OP_KERNEL_ARG_ENABLED_TYPE_LIST(Op, ArgDirection, ArgIndex),   \
      std::tuple>
