#ifndef _INC_VIDEOCOMMUN_HPP
#define _INC_VIDEOCOMMUN_HPP

#include "../Common.hpp"

#include <cmath>
#include <random>
#include <algorithm>
#include <queue>
#include <deque>
#include <mutex>
#include <sstream>
#include <iomanip>



extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
}

#endif // !_INC_VIDEOCOMMUN_HPP
