#ifndef TEXENGINE_BUFFER_HPP_INCLUDED
#define TEXENGINE_BUFFER_HPP_INCLUDED

namespace TEXEngine {
	namespace Util {

template <class T>
class Buffer
{
	public:
		Buffer();
		Buffer(const Buffer &rhs);
		Buffer &operator =(const Buffer &rhs);
		virtual ~Buffer();

		int init(unsigned int count);
		void clear();

		T *data() const;
		unsigned int count() const;

	private:
		void release();

		unsigned int m_count;
		T * m_data;
};

	}/* namespace Util */
} /* Namespace TEXEngine */

#include "buffer.tml"

#endif /* TEXENGINE_BUFFER_HPP_INCLUDED */