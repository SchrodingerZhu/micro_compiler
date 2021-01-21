//
// Created by schrodinger on 1/21/21.
//

#ifndef MICRO_COMPILER_GRAMMAR_H
#define MICRO_COMPILER_GRAMMAR_H

#include <frontend.h>
namespace grammar {
    struct Primary;
    struct Digit;
    struct Toplevel;
    struct Expr;
    using namespace parser;

    RULE(Op, Ord<Char<'+'>, Char<'-'>>)

    RULE(MicroBegin, Keyword<'b', 'e', 'g', 'i', 'n'>)

    RULE(MicroEnd, Keyword<'e', 'n', 'd'>)

    RULE(Read, Keyword<'r', 'e', 'a', 'd'>)

    RULE(Write, Keyword<'w', 'r', 'i', 't', 'e'>)

    RULE(Alpha, Ord<CharRange<'a', 'z'>, CharRange<'A', 'Z'>>)

    RULE(Digit, CharRange<'0', '9'>)

    RULE(Integer, Plus<Digit>)

    RULE(ASCII, Ord<Alpha, Digit, Char<'_'>>)

    RULE(Identity, Seq<Ord<Alpha, Char<'_'>>, Asterisk<ASCII>>)

    RULE(AssignmentOp, Seq<Char<':'>, Char<'='>>)

    RULE(Assignment, SpaceInterleaved<Identity, AssignmentOp, Expr>)

    RULE(IdentityList, SpaceInterleaved<Identity, Asterisk<SpaceInterleaved<Char<','>, IdentityList>>>);

    RULE(ExprList, SpaceInterleaved<Expr, Asterisk<SpaceInterleaved<Char<','>, ExprList>>>);

    RULE(ReadStmt, SpaceInterleaved<Read, Char<'('>, IdentityList, Char<')'>>)

    RULE(WriteStmt, SpaceInterleaved<Write, Char<'('>, ExprList, Char<')'>>)

    RULE(Primary, Ord<SpaceInterleaved<Char<'('>, Expr, Char<')'>>, Identity, Integer>)

    RULE(Expr, Ord<SpaceInterleaved<Primary, Asterisk<SpaceInterleaved<Op, Primary>>>>)

    RULE(Stmt, SpaceInterleaved<Ord<ReadStmt, WriteStmt, Assignment>, Char<';'>>)

    RULE(Toplevel, SpaceInterleaved<Start, MicroBegin, Plus<Stmt>, MicroEnd, End>)

    using SelectRule = Selector<Toplevel, ReadStmt, WriteStmt, Expr, Primary, Identity, Assignment, Integer, Op>;

}

#endif //MICRO_COMPILER_GRAMMAR_H
