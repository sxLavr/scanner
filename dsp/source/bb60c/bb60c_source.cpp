
#include "dsp/bb60c/bb60c_source.h"

#include "bb60_sdk/include/bb_api.h"
#include <gnuradio/io_signature.h>
#include <math.h>
#include <map>


namespace DSP
{

    namespace
    {
        float const MaxSampleRate = 40e6;

        struct DecimationBandwidthT
        {
            int    decimation;
            double bandwidth;
        };

        DecimationBandwidthT db_table[] = {
            { 1,   27e6 },
            { 2,   17.9e6 },
            { 4,   8e6 },
            { 8,   3.75e6 },
            { 16,  2.0e6 },
            { 32,  1.0e6 },
            { 64,  0.5e6 },
            { 128, 125e3 },
            { 256, 140e3 }
        };
    }

    bb60c_source::bb60c_source( int decimation, float freq )
        : gr::sync_block(
              "bb60c_source",
              gr::io_signature::make( 0, 0, 0 ),
              gr::io_signature::make( 1, 1, 2 * sizeof( float ) )
              )
        , _device_open( false )
        , _decimation( decimation )
        , _freq( freq )
        , _buf( NULL )
    {

        LogOut( Info, "ctor" );

        if (!connect_device())
        {
            throw std::runtime_error( "Can't connect to device" );
        }
        if (!start_device())
        {
            throw std::runtime_error( "Can't start device" );
        }
        int buf_len = 0;
        double bandwidth = 0.0;
        int samples_per_sec = 0;
        bbQueryStreamInfo( _device_num, &buf_len, &bandwidth, &samples_per_sec );

        _buf_len = buf_len * 2;
        _buf = new float [_buf_len];
        _offset = _buf_len;

        message_port_register_in( pmt::intern( "set_freq" ) );
        set_msg_handler(
                    pmt::intern( "set_freq" ),
                    boost::bind( &bb60c_source::set_freq_from_pmt, this, _1 )
                    );
    }


    bb60c_source::~bb60c_source()
    {
        stop_device();
        delete [] _buf;
        disconnect_device();
    }


    int bb60c_source::work( int noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items )
    {
        bbIQPacket pkt;
        pkt.iqData = static_cast<float*>( output_items[0] );
        pkt.iqCount = noutput_items;
        pkt.triggers = NULL;
        pkt.triggerCount = 0;
        pkt.purge = BB_FALSE;

        bbGetIQ( _device_num, &pkt );

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }


    void bb60c_source::set_freq( float freq )
    {
        _freq = freq;
        //stop_device();
        //start_device();
    }


    void bb60c_source::set_freq_from_pmt(pmt::pmt_t msg)
    {
        set_freq( pmt::to_float( msg ) );
    }


    bb60c_source::sptr bb60c_source::make( int decimation, float freq )
    {
        return gnuradio::get_initial_sptr( new bb60c_source( decimation, freq ) );
    }


    bool bb60c_source::connect_device()
    {
        if (_device_open)
            return true;
        bbStatus status = bbOpenDevice( &_device_num );
        if (status != bbNoError)
            return false;
        _device_open = true;
        return true;
    }


    void bb60c_source::disconnect_device()
    {
        if (!_device_open)
            return;
        _device_open = false;
        ::sleep( 1 );
        bbCloseDevice( _device_num );
    }


    bool bb60c_source::start_device()
    {
        // Configure device
        bbConfigureLevel( _device_num, 0.0, 30.0 );
        bbConfigureCenterSpan( _device_num, _freq, 20.0e6 );
        bbConfigureGain( _device_num, 2 );

        double bandwidth;
        if (!find_bandwidth( bandwidth ))
        {
            return false;
        }
        bbConfigureIQ( _device_num, _decimation, bandwidth );

        // Initiate raw mode
        bbStatus status = bbInitiate( _device_num, BB_STREAMING, BB_STREAM_IQ );

        return (status == bbNoError);
    }


    void bb60c_source::stop_device()
    {
        bbAbort( _device_num );
    }


    bool bb60c_source::find_bandwidth( double &bandwidth )
    {
        for ( int i = 0; i < sizeof( db_table ) / sizeof( DecimationBandwidthT ); ++i )
        {
            if (_decimation == db_table[i].decimation)
            {
                bandwidth = db_table[i].bandwidth;
                return true;
            }
        }
        return false;
    }


    TACKLE_IMPLEMENT_LOGGER( bb60c_source )

} // namespace DSP
