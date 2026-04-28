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
	Is,
	ModOp,
	CModOp, // 40
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
	Not,
	Comma,
	This
};

static std::string TokenTypeToString(const NovaTokenType& type) {
	switch (type) {
	case NovaTokenType::IntLit:       return "IntLit";
	case NovaTokenType::FloatLit:     return "FloatLit";
	case NovaTokenType::StringLit:    return "StringLit";
	case NovaTokenType::BoolLit:      return "BoolLit";
	case NovaTokenType::OpenParen:    return "OpenParen";
	case NovaTokenType::CloseParen:   return "CloseParen";
	case NovaTokenType::OpenBracket:  return "OpenBracket";
	case NovaTokenType::CloseBracket: return "CloseBracket";
	case NovaTokenType::Identifier:   return "Identifier";
	case NovaTokenType::PlusOp:       return "PlusOp";
	case NovaTokenType::MinusOp:      return "MinusOp";
	case NovaTokenType::MultOp:       return "MultOp";
	case NovaTokenType::DivOp:        return "DivOp";
	case NovaTokenType::EqualOp:      return "EqualOp";
	case NovaTokenType::OrOp:         return "OrOp";
	case NovaTokenType::AndOp:        return "AndOp";
	case NovaTokenType::Assignment:   return "Assignment";
	case NovaTokenType::CPlusOp:      return "CPlusOp";
	case NovaTokenType::CMinusOp:     return "CMinusOp";
	case NovaTokenType::CMultOp:      return "CMultOp";
	case NovaTokenType::CDivOp:       return "CDivOp";
	case NovaTokenType::VarDecl:      return "VarDecl";
	case NovaTokenType::FuncDecl:     return "FuncDecl";
	case NovaTokenType::IfStmt:       return "IfStmt";
	case NovaTokenType::ElseStmt:     return "ElseStmt";
	case NovaTokenType::ElifStmt:     return "ElifStmt";
	case NovaTokenType::GreaterOp:    return "GreaterOp";
	case NovaTokenType::LesserOp:     return "LesserOp";
	case NovaTokenType::GtetOp:       return "GtetOp";
	case NovaTokenType::LtetOp:       return "LtetOp";
	case NovaTokenType::Eof:          return "Eof";
	case NovaTokenType::TypeDecl:     return "TypeDecl";
	case NovaTokenType::Mirror:       return "Mirror";
	case NovaTokenType::Include:      return "Include";
	case NovaTokenType::Return:       return "Return";
	case NovaTokenType::Break:        return "Break";
	case NovaTokenType::Continue:     return "Continue";
	case NovaTokenType::Dot:          return "Dot";
	case NovaTokenType::Is:           return "Is";
	case NovaTokenType::ModOp:        return "ModOp";
	case NovaTokenType::CModOp:       return "CModOp";
	case NovaTokenType::BreakPoint:   return "BreakPoint";
	case NovaTokenType::NewLine:      return "NewLine";
	case NovaTokenType::Const:        return "Const";
	case NovaTokenType::ASTPrint:     return "ASTPrint";
	case NovaTokenType::For:          return "For";
	case NovaTokenType::In:           return "In";
	case NovaTokenType::While:        return "While";
	case NovaTokenType::OpenBrace:    return "OpenBrace";
	case NovaTokenType::CloseBrace:   return "CloseBrace";
	case NovaTokenType::Null:         return "Null";
	case NovaTokenType::Not:          return "Not";
	case NovaTokenType::Comma:        return "Comma";
	case NovaTokenType::This:         return "This";
	default:                          return "UnknownToken";
	}
}

struct Token {
	NovaTokenType type;
	std::string content;
	int line;
	int column;
};


#endif
