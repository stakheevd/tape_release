#include "TapeImpl.h"

TapeImpl::TapeImpl(const std::string& file, TapeConfigurator& config) :
  filename(file),
  configurator(config)
{
  file_stream.open(file, std::ios::in | std::ios::out | std::ios::app);
}

TapeImpl::TapeImpl(TapeImpl&& other) noexcept :
  file_stream(std::move(other.file_stream)),
  filename(std::move(other.filename)),
  configurator(other.configurator)
{
}

TapeImpl& TapeImpl::operator=(TapeImpl&& other) noexcept
{
  if (this != &other)
  {
    file_stream = std::move(other.file_stream);
    filename = std::move(other.filename);
    configurator = std::move(other.configurator);
  }

  return *this;
}

void TapeImpl::write(int32_t value)
{
  auto cur_pos = file_stream.tellp();

  file_stream << value << ' ';
  file_stream.seekp(cur_pos);

  std::this_thread::sleep_for(std::chrono::milliseconds(configurator.get_write_delay()));
}

int32_t TapeImpl::read()
{
  int32_t value = 0;
  auto cur_pos = file_stream.tellg();

  if (!(file_stream >> value))
    throw std::runtime_error("Empty or incorrect input");

  file_stream.seekg(cur_pos);

  std::this_thread::sleep_for(std::chrono::milliseconds(configurator.get_read_delay()));

  return value;
}

void TapeImpl::rewind()
{
  if (!file_stream.is_open())
    throw std::runtime_error("Tape is not open");

  while (file_stream.tellg() > 0 || file_stream.tellp() > 0)
    move_backward();
}

void TapeImpl::move_forward()
{
  file_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
  file_stream.peek();

  std::this_thread::sleep_for(std::chrono::milliseconds(configurator.get_move_delay()));
}

void TapeImpl::move_backward()
{
  if (!file_stream.is_open())
    throw std::runtime_error("Tape is not open");

  if (file_stream.tellg() == 0)
  {
   file_stream.setstate(std::ios::eofbit);
   return;
  }

  while (file_stream.tellg() != 0 && file_stream.peek() != ' ')
    file_stream.seekg(-1, std::ios::cur);

  if (file_stream.peek() == ' ')
  {
    file_stream.seekg(-1, std::ios::cur);

    while (file_stream.tellg() != 0 && file_stream.peek() != ' ')
      file_stream.seekg(-1, std::ios::cur);
  }

  if (file_stream.peek() == ' ')
    file_stream.seekg(1, std::ios::cur);

  std::this_thread::sleep_for(std::chrono::milliseconds(configurator.get_move_delay()));
}

void TapeImpl::open()
{
  if (!file_stream.is_open())
    file_stream.open(filename, std::ios::in | std::ios::out | std::ios::app);
}

void TapeImpl::close()
{
  if (file_stream.is_open())
    file_stream.close();
}

bool TapeImpl::eof()
{
  return file_stream.eof();
}

void TapeImpl::reset_flags()
{
  file_stream.clear();
}

TapeImpl::~TapeImpl() noexcept
{
  if (file_stream.is_open())
    file_stream.close();
}
