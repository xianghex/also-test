/* also: Advanced Logic Synthesis and Optimization tool
 * Copyright (C) 2019- Ningbo University, Ningbo, China */

/**
 * @file lut_resyn.hpp
 *
 * @brief TODO
 *
 * @author Zhufei Chu
 * @since  0.1
 */

#ifndef LUT_RESYN_HPP
#define LUT_RESYN_HPP

#include <mockturtle/mockturtle.hpp>
#include <mockturtle/algorithms/node_resynthesis/xmg_npn.hpp>

#include "../networks/m5ig/m5ig_npn.hpp"

namespace alice
{
  
  class lut_resyn_command: public command
  {
    public:
      explicit lut_resyn_command( const environment::ptr& env ) 
               : command( env, "lut resyn using optimal networks (default:mig)" )
      {
        add_option( "cut_size, -k", cut_size, "set the cut size from 2 to 8, default = 4" );
        add_flag( "--verbose, -v", "print the information" );
        add_flag( "--xmg, -x", "using xmg as target logic network" );
        add_flag( "--m5ig, -r", "using m5ig as target logic network" );
        add_flag( "--new_entry, -n", "adds new store entry" );
      }

      rules validity_rules() const
      {
        return { has_store_element<klut_network>( env ) };
      }


    protected:
      void execute()
      {
        /* derive some AIG */
        klut_network klut = store<klut_network>().current();

        /* lut resynthesis */
        if( is_set( "xmg" ) )
        {
          xmg_npn_resynthesis resyn;
          const auto xmg = node_resynthesis<xmg_network>( klut, resyn );

          depth_view xmg_depth{xmg};
          std::cout << "[I/O:" << xmg.num_pis() << "/" << xmg.num_pos() << "] XMG gates: " 
                    << xmg.num_gates() << " XMG depth: " << xmg_depth.depth() << std::endl;

          /* add to store */
          if( is_set( "new_entry" ) )
          {
            store<xmg_network>().extend(); 
            store<xmg_network>().current() = xmg;
          }
        }
        else if( is_set( "m5ig" ) )
        {
          m5ig_npn_resynthesis resyn;
          const auto m5ig = node_resynthesis<m5ig_network>( klut, resyn );

          depth_view m5ig_depth{m5ig};
          std::cout << "[I/O:" << m5ig.num_pis() << "/" << m5ig.num_pos() << "] M5IG gates: " 
                    << m5ig.num_gates() << " M5IG depth: " << m5ig_depth.depth() << std::endl;

          /* add to store */
          if( is_set( "new_entry" ) )
          {
            store<m5ig_network>().extend(); 
            store<m5ig_network>().current() = m5ig;
          }
        }
        else
        {
          mig_npn_resynthesis resyn;
          const auto mig = node_resynthesis<mig_network>( klut, resyn );

          depth_view mig_depth{mig};
          std::cout << "[I/O:" << mig.num_pis() << "/" << mig.num_pos() << "] MIG gates: " 
                    << mig.num_gates() << " MIG depth: " << mig_depth.depth() << std::endl;
          
          /* add to store */
          if( !is_set( "notnew" ) )
          {
            store<mig_network>().extend(); 
            store<mig_network>().current() = mig;
          }
        }
      }

    private:
      int cut_size = 4;
  };

  ALICE_ADD_COMMAND( lut_resyn, "Exact synthesis" )


}

#endif
