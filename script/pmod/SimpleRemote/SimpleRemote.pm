package SimpleRemote;

use Exporter;
@ISA = qw(Exporter);
@EXPORT = qw(
	rGetFile
	rPutFile
	rRemoteCommand
	vReboot
);

sub prErrExit($) { my (
	$message	# message for error
    ) = @_;
	print "ERROR : $message\n";
	die "";
}

$ExecutingPid=0;	# pid of command executing

sub checkChild(){
	$ChildPid    = wait;
	$ChildStatus = $?; 	# save child status
	my ($HexStatus) = sprintf("0x%08x",$ChildStatus);
	#print("Catch child die pid=$ChildPid status=$HexStatus\n");

	$ExecutingPid=0 if($ChildPid==$ExecutingPid); # execCmd() finished
}

sub getChildStatus(){
	$ChildStatus;
}

sub searchPath($$) { my(
        $path,           # path
        $filename        # filename for search
    ) = @_;
	my $fullname="";
	if($filename =~ m!.*/.+!) {
		$fullname=$filename;
        }
	my @paths=split(/:/,($path));
		foreach(@paths) {
			my $tmpname=$_ . "/" . $filename;
			#my $tmpname=$_ .  $filename;
			if($fullname eq "") {
				$fullname=$tmpname if(-r $tmpname);
			}
	}
	prErrExit("$filename don't exist in the path or cannot read in ``$path''\n") if($fullname eq "");
	$fullname;
}

sub vRemote($;$@) { my (
	$fname,		# remote file name
	$opts,		# options
	@args,		# variable args
    ) = @_;

	# Set SIGCHLD handler to check pktbuf died while testing
	$SIG{CHLD} = \&checkChild;

	$V6Root = "/usr/local/v6eval";
	$System = $config{'System'};

	my $rpath = "./";
	$rpath .= ":./$System/";
	$rpath .= ":$V6Root/bin/$System/";
	$rpath .= ":$V6Root/bin/unknown/";

	my $cmd  = searchPath($rpath, $fname);
	$cmd .= " -t $System";
	$cmd .= " -u $config{'User'}";
	$cmd .= " -p $config{'Password'}";
	$cmd .= " -d $config{'RemoteDevice'}";
	$cmd .= " -v $config{'RemoteDebug'}";
	$cmd .= " -i $config{'RemoteIntDebug'}";
	$cmd .= " -o $config{'RemoteLog'}";
	$cmd .= " $RemoteOption $opts @args";

	pipe PIN,COUT;
	$ChildPid = 0;
	my $localExecutingPid;
	unless(defined($ExecutingPid = $localExecutingPid = fork)){
		$localExecutingPid=0;
		prErrExit("Fork failed for <$cmd>");
	}
	unless( $localExecutingPid ) { # Children
		open(STDOUT, ">&COUT");
		open(STDERR, ">&COUT");
		close(PIN);
		# close(STDIN);
		# should close if any explicit open files exit
		exec "$cmd";
		prErrExit("Exec failed for <$cmd>");
	}

	my $pid=$localExecutingPid;
	close(COUT);

	$stdout = '';
	while( <PIN> ) {
		$stdout .= $_;
		chomp;
		print("$_\n");
	}
	close(PIN);
	while($ChildPid == 0){};
	my $status = getChildStatus();
	prErrExit("Unknown child died $pid $ChildPid (status=$status)!!") if($ChildPid != $pid);
	if($status & 0xff) { # died by signal
		prErrExit("Catch signal <$cmd>");
	}
	$SIG{CHLD} = IGNORE;
	my $rc=($status >> 8);
	$rc;
}

#======================================================================
# rGetFile - get a file from remote system
#======================================================================
sub rGetFile($$) {
	my ($from, $to) = @_;

	vRemote("getfile.rmt", "from=$from", "to=$to") && return 1;

	return 0;
}

#======================================================================
# rPutFile - put file to NUT
#======================================================================
sub rPutFile($$) {
	my ($from, $to) = @_;

	vRemote("putfile.rmt", "from=$from", "to=$to") && return 1;
	return 0;
}

#======================================================================
# rRemoteCommand - execute command on NUT
#======================================================================
sub rRemoteCommand($) {
	my ($cmd) = @_;

	vRemote("rcommand.rmt", "cmd=\"$cmd\"") && return 1;
	return 0;
}

sub vReboot() {
	vRemote("reboot.rmt", "") && return 1;
	return 0;
}

BEGIN {
	$config{'User'} = "root";
	$config{'Password'} = "rootpassword";
	$config{'System'} = "linux-v6";
	$config{'RemoteDevice'} = "ttyS0";
	$config{'RemoteDebug'} = 0;
	$config{'RemoteIntDebug'} = 0;
	$config{'RemoteLog'} = 0;
}

1;
