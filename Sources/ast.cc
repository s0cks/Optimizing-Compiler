#include "ast.h"

#define DEFINE_VISIT_FUNCTION(BaseName) \
    void BaseName##Node::Visit(AstNodeVisitor* visitor){ \
        visitor->Visit##BaseName##Node(this); \
    }
FOR_EACH_NODE(DEFINE_VISIT_FUNCTION)
#undef DEFINE_VISIT_FUNCTION