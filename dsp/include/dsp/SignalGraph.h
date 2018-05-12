#ifndef __DSP_SIGNALGRAPH_H__
#define __DSP_SIGNALGRAPH_H__


#include "logger.h"

#include <gnuradio/sync_block.h>
#include <gnuradio/top_block.h>


namespace DSP
{

    class SignalGraph
    {
    public:
        SignalGraph( std::string const& name );
        ~SignalGraph();

    private:
        std::string const   _name;
        gr::top_block_sptr  _top_block;

        TACKLE_DECLARE_DEFAULT_LOGGER( SignalGraph )
    };

} // namespace DSP


#endif // __DSP_SIGNALGRAPH_H__
