// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#define KOMP_FORWARD_DECLARE(komp_class_name)                                                       \
class komp_class_name;                                                                              \
using komp_class_name##UPtr = std::unique_ptr<komp_class_name>;                                     \
using komp_class_name##Ref = std::reference_wrapper<komp_class_name>;                               \
                                                                                                    \
using komp_class_name##UPtrVec = std::vector<std::unique_ptr<komp_class_name>>;                     \
using komp_class_name##RefVec = std::vector<std::reference_wrapper<komp_class_name>>;               \
using komp_class_name##s = std::vector<komp_class_name>;                                            \
                                                                                                    \
using Const##komp_class_name##UPtr = std::unique_ptr<const komp_class_name>;                        \
using Const##komp_class_name##UPtrVec = std::vector<std::unique_ptr<const komp_class_name>>;        \
using Const##komp_class_name##Ref = std::reference_wrapper<const komp_class_name>;                  \
using Const##komp_class_name##RefVec = std::vector<std::reference_wrapper<const komp_class_name>>;  \
using Const##komp_class_name##s = std::vector<const komp_class_name>;

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
