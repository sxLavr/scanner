
#include "dsp/SignalGraph.h"

namespace DSP
{

    SignalGraph::SignalGraph( std::string const& name )
        : _name( name )
        , _top_block( gr::make_top_block( _name ) )
    {
        LogOut( Info, "ctor" );
    }


    SignalGraph::~SignalGraph()
    {

    }


    TACKLE_IMPLEMENT_LOGGER( SignalGraph )

} // namespace DSP
