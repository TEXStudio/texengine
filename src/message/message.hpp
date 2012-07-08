#include	<vector>

#ifndef		TEXENGINE_MESSAGE_HPP_INCLUDED
	#define	TEXENGINE_MESSAGE_HPP_INCLUDED
	
	
	
	namespace	TEXEngine
	{
		namespace	Core
		{

			typedef	long long	MESSAGE_INTEGRAL_DATA_TYPE;
			typedef	double		MESSAGE_REAL_DATA_TYPE;


			/*	
				Basic communication class. Widely used by the message system of the game engine.
				It uses two arrays, created at the initialisation of the class, to communicate
				data and pointers from one engine Object to another.
			*/
			class	Message
			{
				private:
				
				
					std::vector<MESSAGE_INTEGRAL_DATA_TYPE>					_integral_data;
					std::vector<MESSAGE_REAL_DATA_TYPE>						_real_data;
					std::vector<void*>										_pointers;


				public:

					// 
					explicit Message( const unsigned long integral_data_size = 0 , const unsigned long real_data_size = 0 , const unsigned long pointers_size = 0 );
					~Message();


					//	Add data of size equal to the size of long double to the integral data array, at position index.
					void													add_integral_data( const unsigned long index , const MESSAGE_INTEGRAL_DATA_TYPE data );
					//	Remove the data stored at index position in the integral data array.
					void													remove_integral_data( const unsigned long index );
					//	Erase all data stored within the integral data array. All data values are set to zero.
					void													clear_integral_data();

					//	Add data of size equal to the size of long double to the integral data array, at position index.
					void													add_real_data( const unsigned long index , const MESSAGE_REAL_DATA_TYPE data );
					//	Remove the data stored at index position in the integral data array.
					void													remove_real_data( const unsigned long index );
					//	Erase all data stored within the integral data array. All data values are set to zero.
					void													clear_real_data();

					//	Make the pointer at the index position of the pointer array, point where the pointer parameter is pointing.
					void													add_pointer( const unsigned long index , void* pointer );
					//	Remove the pointer stored at the index position of the pointer array.
					void													remove_pointer( const unsigned long index );
					//	Erase all pointers stored within the pointer array. All pointer values are set to NULL.
					void													clear_pointers();


					//	Get the size of the integral data array.
					unsigned long											get_integral_data_size() const;
					//	Get the size of the real data array.
					unsigned long											get_real_data_size() const;
					//	Get the size of the pointers array.
					unsigned long											get_pointers_size() const;
					//	Get a pointer to the whole integral data array.
					std::vector<MESSAGE_INTEGRAL_DATA_TYPE>::const_iterator	get_integral_data() const;
					//	Get the contents of the integral data array at the index location.
					MESSAGE_INTEGRAL_DATA_TYPE								get_integral_data( const unsigned long index ) const;
					//	Get a pointer to the whole real data array.
					std::vector<MESSAGE_REAL_DATA_TYPE>::const_iterator		get_real_data() const;
					//	Get the contents of the real data array at the index location.
					MESSAGE_REAL_DATA_TYPE									get_real_data( const unsigned long index ) const;
					//	Get a pointer to the whole pointer array.
					std::vector<void*>::const_iterator						get_pointers() const;
					//	Get the contents of the pointer array at the index location.
					void*													get_pointer( const unsigned long index ) const;
			};



			/*
				Function Implementations
			*/


			inline	Message::Message( const unsigned long integral_data_size , const unsigned long real_data_size , const unsigned long pointers_size )	:	
				_integral_data(integral_data_size,0) , _real_data(real_data_size,0.0) , _pointers(pointers_size,NULL)	{};

			inline	Message::~Message()	{};


			inline void	Message::add_integral_data( const unsigned long index , const MESSAGE_INTEGRAL_DATA_TYPE data )
			{
				if ( this->_integral_data.size() > index )
					this->_integral_data[index] = data;
			};

			inline void	Message::remove_integral_data( const unsigned long index )
			{
				if ( this->_integral_data.size() > index )
					this->_integral_data[index] = 0;
			};

			inline void	Message::clear_integral_data()
			{
				unsigned int	temp = this->_integral_data.size();



				this->_integral_data.clear();
				this->_integral_data.resize(temp,0);
			};


			inline void	Message::add_real_data( const unsigned long index , const MESSAGE_REAL_DATA_TYPE data )
			{
				if ( this->_real_data.size() > index )
					this->_real_data[index] = data;
			};

			inline void	Message::remove_real_data( const unsigned long index )
			{
				if ( this->_real_data.size() > index )
					this->_real_data[index] = 0.0;
			};

			inline void	Message::clear_real_data()
			{
				unsigned int	temp = this->_real_data.size();



				this->_real_data.clear();
				this->_real_data.resize(temp,0);
			};


			inline void	Message::add_pointer( const unsigned long index , void* pointer )
			{
				if ( this->_pointers.size() > index )
					this->_pointers[index] = pointer;
			};

			inline void	Message::remove_pointer( const unsigned long index )
			{
				if ( this->_pointers.size() > index )
					this->_pointers[index] = NULL;
			};

			inline void	Message::clear_pointers()
			{
				unsigned int	temp = this->_pointers.size();



				this->_pointers.clear();
				this->_pointers.resize(temp,NULL);
			};


			inline unsigned long											Message::get_integral_data_size() const		{ return this->_integral_data.size(); };
			inline unsigned long											Message::get_real_data_size() const			{ return this->_real_data.size(); };
			inline unsigned long											Message::get_pointers_size() const			{ return this->_pointers.size(); };
			inline std::vector<MESSAGE_INTEGRAL_DATA_TYPE>::const_iterator	Message::get_integral_data() const			{ return this->_integral_data.begin(); };
			inline MESSAGE_INTEGRAL_DATA_TYPE								Message::get_integral_data( const unsigned long index ) const
			{
				if ( index < this->_integral_data.size() )
					return this->_integral_data[index];
				else
					return 0;
			};

			inline std::vector<MESSAGE_REAL_DATA_TYPE>::const_iterator		Message::get_real_data() const				{ return this->_real_data.begin(); };
			inline MESSAGE_REAL_DATA_TYPE									Message::get_real_data( const unsigned long index ) const
			{
				if ( index < this->_real_data.size() )
					return this->_real_data[index];
				else
					return 0;
			};

			inline std::vector<void*>::const_iterator						Message::get_pointers() const				{ return this->_pointers.begin(); };
			inline void*													Message::get_pointer( const unsigned long index ) const
			{
				if ( index < this->_pointers.size() )
					return this->_pointers[index];
				else
					return NULL;
			};

		}	/* Core */
	}	/* TEXEngine */



#endif		/* TEXENGINE_MESSAGE_HPP_INCLUDED */