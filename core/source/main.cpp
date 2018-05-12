
#include <boost/log/trivial.hpp>

//#include "dsp/bb60c/bb60c_source.h"

//#include <gnuradio/blocks/udp_sink.h>
//#include <gnuradio/blocks/throttle.h>
//#include <gnuradio/top_block.h>

//#include <gnuradio/uhd/usrp_source.h>

//#include <gnuradio/fft/fft_vcc.h>
//#include <gnuradio/blocks/stream_to_vector.h>
//#include <gnuradio/blocks/vector_to_stream.h>

//#include <gnuradio/filter/rational_resampler_base_fff.h>
//#include <gnuradio/filter/freq_xlating_fir_filter_ccf.h>
//#include <gnuradio/filter/firdes.h>
//#include <gnuradio/filter/fractional_resampler_ff.h>
//#include <gnuradio/analog/quadrature_demod_cf.h>
//#include <gnuradio/audio/sink.h>


#include "dsp/SignalGraph.h"

#include "logger.h"



namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

int main()
{
    LogOut( Info, "Start the scanner" );


    DSP::SignalGraph sg( "weferfe" );










/*
    float const freq = 936e6;

    int const bb60c_decimation = 4;

    float const sample_rate = 40e6 / bb60c_decimation;
    int const filter_decimation = 1;
    int const audio_sample_rate = 48000;
    float const resamp_ratio = sample_rate / filter_decimation / audio_sample_rate;

    DSP::bb60c_source::sptr bb60c_src = DSP::bb60c_source::make( bb60c_decimation, freq );

    std::string device_addr;
    gr::uhd::usrp_source::sptr usrp_src = gr::uhd::usrp_source::make( device_addr, uhd::stream_args_t( "fc32" ) );
    usrp_src->set_samp_rate( sample_rate );
    usrp_src->set_center_freq( freq );


    gr::blocks::throttle::sptr throttle = gr::blocks::throttle::make( 4 * 2, sample_rate );

//    gr::blocks::stream_to_vector::sptr s2v = gr::blocks::stream_to_vector::make( 4 *2, 1024 );

//    std::vector<float> window = gr::fft::window::hamming( 1024 );
//    gr::fft::fft_vcc::sptr fft = gr::fft::fft_vcc::make( 1024, true, window );

//    gr::blocks::vector_to_stream::sptr v2s = gr::blocks::vector_to_stream::make( 4 * 2, 1024 );

    gr::blocks::udp_sink::sptr udp_sink = gr::blocks::udp_sink::make(
                4 * 2,
                "127.0.0.1",
                4789,
                1024
                );

//    gr::filter::freq_xlating_fir_filter_ccf::sptr filter = gr::filter::freq_xlating_fir_filter_ccf::make(
//                filter_decimation,
//                gr::filter::firdes::low_pass( 1, sample_rate, 250.0e3, 270.0e3, gr::filter::firdes::WIN_HAMMING, 6.76 ),
//                0,
//                sample_rate
//                );

//    gr::analog::quadrature_demod_cf::sptr         demodulator = gr::analog::quadrature_demod_cf::make( 1 );
//    gr::filter::fractional_resampler_ff::sptr     resampler = gr::filter::fractional_resampler_ff::make( 0.0, resamp_ratio );
//    gr::audio::sink::sptr                         audio_sink = gr::audio::sink::make( audio_sample_rate, "" );

    gr::top_block_sptr tb = gr::make_top_block( "scanner" );

//    tb->connect( bb60c_src, 0, throttle, 0 );
//    tb->connect( throttle, 0, udp_sink, 0 );


    tb->connect( usrp_src, 0, throttle, 0 );
    tb->connect( throttle, 0, udp_sink, 0 );


//    tb->connect( bb60c_src, 0, throttle, 0 );
//    tb->connect( throttle, 0, s2v, 0 );
//    tb->connect( s2v, 0, fft, 0 );
//    tb->connect( fft, 0, v2s, 0 );
//    tb->connect( v2s, 0, udp_sink, 0 );

//    tb->connect( bb60c_src, 0, filter, 0 );
//    tb->connect( filter, 0, demodulator, 0 );
//    tb->connect( demodulator, 0, resampler, 0 );
//    tb->connect( resampler, 0, audio_sink, 0 );

    tb->start();

    while (1)
    {
        sleep( 1 );
    }

    sleep( 10 );
    tb->stop();
*/
    return 0;
}
