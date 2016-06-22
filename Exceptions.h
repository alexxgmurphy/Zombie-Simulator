#pragma once
#include <exception>

class InvalidOp : public std::exception
{
public:
	const char* what() const noexcept override { std::cout << "Invalid Op Exception Thrown" << std::endl; return "Invalid OP code or parameters."; }
};

class FileOpenErr : public virtual std::exception
{
public:
	const char* what() const noexcept override { return "ZOM file exists but could not be opened."; };
};

class FileLoadErr : public virtual std::exception
{
public:
	const char* what() const noexcept override { return "Error loading ZOM file."; };
};