#
# Copyright (C) IBM Corporation 2008.
# All rights reserved.
#
# Redistribution and use of this software in source and binary forms, with
# or without modification, are permitted provided that the following
# conditions and disclaimer are agreed and accepted by the user:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the names of the copyrighters, the name of the project which
# is related to this software (hereinafter referred to as "project") nor
# the names of the contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# 4. No merchantable use may be permitted without prior written
# notification to the copyrighters. However, using this software for the
# purpose of testing or evaluating any products including merchantable
# products may be permitted without any notification to the copyrighters.
#
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHTERS, THE PROJECT AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING
# BUT NOT LIMITED THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.  IN NO EVENT SHALL THE
# COPYRIGHTERS, THE PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT,STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
#    Author: Mitsuru Chinen <chinen@jp.ibm.com>
#
########################################################################

package linux_common;
use Exporter;
@ISA = qw(Exporter);

@EXPORT = qw(
	linux_wait_tty
	linux_rLogout
);

# init process stop respawning agetty in 5 minutes when agetty
# is respawned 10 times in 2 minutes. Then wait should be 13[sec]
my($logout_wait) = 13;
my($hvsi_file) = "/tmp/tahi_hvsi";
my($nonhvsi_file) ="/tmp/tahi_nohvsi";

use V6evalRemote;

sub linux_wait_tty()
{
    if (-e $hvsi_file) {
	# print "Wait in $logout_wait [sec]\n";
	sleep($logout_wait);
   } elsif (! -e $nonhvsi_file) {
	my(@lines, $tty, $is_hvsi);
	rCommand("ps a | awk '{ print \$2 }'", $timeout) || goto error;
	@lines = rCmdOutput();
	foreach $tty (@lines) {
	    $_ = $tty;
	    if (/hvsi/) {
		# print "Wait in $logout_wait [sec]\n";
		sleep($logout_wait);
		open TEMP_FILE, "> $hvsi_file";
		close TEMP_FILE;
	    } else {
		open TEMP_FILE, "> $nonhvsi_file";
		close TEMP_FILE;
	    }
	}
    }
    return(1);
error:
    return(0);
}

sub linux_rLogout($)
{
    my($ret);
    linux_wait_tty() || goto error;
    $ret = rLogout($timeout);
    return($ret);
error:
    return(0);
}
