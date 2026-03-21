#ifndef NOVASCRIPT_TOKEN_H
#define NOVASCRIPT_TOKEN_H

#include <string>

enum class NovaTokenType {
	IntLit,// 0 
	FloatLit,
	StringLit,
	BoolLit,
	OpenParen,
	CloseParen, // 5
	OpenBracket,
	CloseBracket,
	Identifier,
	PlusOp,
	MinusOp, // 10
	MultOp,
	DivOp,
	EqualOp,
	OrOp,
	AndOp, // 15
	Assignment,
	CPlusOp,
	CMinusOp,
	CMultOp,
	CDivOp, // 20
	VarDecl,
	FuncDecl,
	IfStmt,
	ElseStmt,
	ElifStmt, // 25
	GreaterOp,
	LesserOp,
	GtetOp,
	LtetOp,
	Eof, // 30
	TypeDecl,
	Mirror,
	Include,
	Return,
	Break, // 35
	Continue,
	Dot,
	VectorLit2,
	VectorLit3,
	VectorLit4, // 40
	BreakPoint,
	NewLine,
	Const,
	ASTPrint,
	For, // 45
	In,
	While,
	OpenBrace,
	CloseBrace,
	Null, // 50
	Not
};

struct Token {
	NovaTokenType type;
	std::string content;
	int line;
	int column;
};


#endif
