#ifndef __DSP_BB60C_SOURCE_H__
#define __DSP_BB60C_SOURCE_H__


#include <gnuradio/sync_block.h>
#include "logger.h"


namespace DSP
{

    class bb60c_source
            : virtual public gr::sync_block
    {
    public:
        typedef boost::shared_ptr<bb60c_source> sptr;

        bb60c_source( int decimation, float freq );
        ~bb60c_source();

        // gr::sync_block
        virtual int work( int noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items );

        void set_freq( float freq );
        void set_freq_from_pmt( pmt::pmt_t msg );

        /*!
       * \brief Return a shared_ptr to a new instance of bb60c::bb60c_source.
       *
       * To avoid accidental use of raw pointers, bb60c::bb60c_source's
       * constructor is in a private implementation
       * class. bb60c::bb60c_source::make is the public interface for
       * creating new instances.
       */
        static sptr make( int decimation, float freq );

    private:
        bool connect_device();
        void disconnect_device();
        bool start_device();
        void stop_device();
        bool find_bandwidth( double& bandwidth );

        int    _device_num;
        bool   _device_open;
        int    _decimation;
        float  _freq;
        int    _buf_len;
        int    _offset;
        float* _buf;

        TACKLE_DECLARE_DEFAULT_LOGGER( bb60c_source )
    };

} // namespace DSP


#endif // __DSP_BB60C_SOURCE_H__
