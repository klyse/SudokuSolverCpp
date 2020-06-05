#pragma once
#include <ostream>
#include <fstream>

// https://stackoverflow.com/questions/32094980/reading-a-file-using-fstream

template <class T>
void WriteToFile(std::string const& path, T const& value, std::ios_base::openmode mode)
{
	if (auto stream = std::ofstream(path, mode))
	{
		WriteToFile(stream, value);

		stream.close();
	}
	else
	{
		throw std::runtime_error("failed to create/open stream");
	}
}

template <class T>
void WriteToFile(std::ostream& stream, T const& value)
{
	std::copy(value.begin(), value.end(), std::ostreambuf_iterator<char>(stream));

	if (!stream)
	{
		throw std::runtime_error("failed to write");
	}
}

template <class T>
void ReadFromFile(std::istream& stream, T& output)
{
	auto eof = std::istreambuf_iterator<char>();

	output = T(std::istreambuf_iterator<char>(stream), eof);

	if (!stream)
	{
		throw std::runtime_error("failed to read stream");
	}
}

template <class T>
void ReadFromFile(std::string const& path, T& output)
{
	if (auto stream = std::ifstream(path, std::ios::in | std::ios::binary))
	{
		ReadFromFile(stream, output);

		stream.close();
	}
	else
	{
		throw std::runtime_error("failed to create stream");
	}
}
