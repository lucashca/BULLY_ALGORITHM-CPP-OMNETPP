//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package bully.simulations;


//
// Two instances (tic and toc) of Txc connected.
//

import bully.Node;



network Bully
{
    parameters:
        int numberOfNodes = default(10);
        @display("i=block/routing");

    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
        channel Channel2 extends ned.DelayChannel
        {
            delay = 150ms;
        }
    submodules:
        no[numberOfNodes]: Node;
    connections:



        for i=0..numberOfNodes-2, for j=0..numberOfNodes-2, if j<i {
            no[i].out[j] --> Channel --> no[j].in[i];

        }

        for i=0..numberOfNodes-2, for j=0..numberOfNodes-2, if j>=i {
            no[i].out[j] --> Channel2 --> no[j+1].in[i];

        }

        for i=0..numberOfNodes-2 {
            no[numberOfNodes-1].out[i] --> Channel --> no[i].in[i];

        }











}
