<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>PADmaps.org - Protocol</title>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<meta name="Author" content="Andrey Aleksanyants, M.D." />
<meta name="Keywords" content="PADmap, pad, ASP, PAD extension" />
<meta name="Description" content="PADmap format, protocol, usage" />
<link rel="stylesheet" type="text/css" href="/padmaps.css" />
</head>
<body>
<div class="header">
<h1>PADmaps.org</h1>
</div>
<ul class="tabs">
	<li><a href="/">Home</a></li>
	<li class="current"><a href="/protocol.php">Protocol</a></li>
	<li><a href="/repository.php">Repository</a></li>
	<li><a href="/resources.php">Resources</a></li>
	<li><a href="/buttons.php">Buttons</a></li>
	<li><a href="/faq.php">FAQ</a></li>
	<li><a href="/terms.php">Terms</a></li>
</ul>
<div class="main">
	<ul class="sidebar">
		<li><a href="#protocol_format">PADmap format</a></li>
		<li><a href="#protocol_sample">Sample PADmap</a></li>
		<li><a href="#protocol_location">Specifying PADmap location</a></li>
		<li><a href="#protocol_server">Server configuration</a></li>
	</ul>
	<div class="content">
		<h2 id="protocol">Protocol</h2>
		<h3 id="protocol_format">PADmap format</h3>
		<p>The PADmap format consists of full PAD URLs in plain text mode, one URL per row. No other types of text (comments, empty rows, html tags, javascript blocks etc.) are allowed. All PAD URLs in a PADmap must be from a single host, such as <code>www.yourdomain.com</code> or <code>subdomain.yourdomain.com</code>. This is to ensure that all PAD URLs belong to the same author.</p>
		<p class="backtotop"><a href="#protocol">Back to top</a></p>
		
		<h3 id="protocol_sample">Sample PADmap</h3>
		<p>The following example shows a PADmap that contains 2 PAD URLs.</p><pre>
http://www.yourdomain.com/pads/software1_pad.xml
http://www.yourdomain.com/pads/software2_pad.xml</pre>
		<p class="backtotop"><a href="#protocol">Back to top</a></p>
		
		<h3 id="protocol_location">Specifying PADmap location</h3>
		<p>The location of PADmap should be specified inside PAD files from the same host. This will allow PAD-enabled download sites to know it by checking PADs in their databases for the appropriate field value, pick up your PADmap file and check the existing PAD URLs against PAD URLs from PADmap file to add missing ones to their databases.</p>
		<p>The following node and its sub-nodes will be added to your PAD XML data under the root <code>&lt;XML_DIZ_INFO&gt;</code> node:</p>
<pre>&lt;PADmap&gt;
    &lt;PADmap_FORM&gt;Y&lt;/PADmap_FORM&gt;
    &lt;PADmap_DESCRIPTION&gt;
        Link to plain text file containing all your PAD URLs from current host
    &lt;/PADmap_DESCRIPTION&gt;
    &lt;PADmap_VERSION&gt;1.0&lt;/PADmap_VERSION&gt;
    &lt;PADmap_URL&gt;http://www.padmaps.org/padmap.htm&lt;/PADmap_URL&gt;
    &lt;PADmap_SCOPE&gt;Company&lt;/PADmap_SCOPE&gt;
    &lt;PADmap_Location&gt;http://www.yourdomain.com/pads/padmap.txt&lt;/PADmap_Location&gt;
&lt;/PADmap&gt;</pre>
		<p>Where <code>&lt;PADmap_Location&gt;</code> specifies the URL of a PADmap containing all your PAD URLs in plain text mode (for other fields descriptions please refer to the official <a href="http://pad.asp-software.org/spec/xtensions_spec.php">PAD extensions specification</a> by ASP-Software.org).</p>
		<p><b>Note:</b> if you store all your PAD files in the single directory on your server and have specified your PADmap as the PAD files' directory index, you can specify the path to your PADs directory instead of the full path to your PADmap, e.g. <code>http://www.yourdomain.com/pads/</code></p>
		<p class="backtotop"><a href="#protocol">Back to top</a></p>

		<h3 id="protocol_server">Server configuration</h3>
		<p>There are no special requirements on PADmap file name and its server placement, however, if you host all your PAD files in the same subdirectory on your server, the best solution would be to place your PADmap right there and to make it the directory index.</p>
		<p>If you use a server-side script for PADmap generation (see PADmap Kit in <a href="/resources.php">Resources</a> section), you do not have to modify your server settings because <code>index.php</code> is accepted as directory index by default.</p>
		<p>Otherwise, if you create your PADmap text file manually, you will have to create/modify <b>.htaccess</b> file in the appropriate subdirectory on Apache based servers by specifying your PADmap file as the directory index file, e.g.:</p>
<pre>
DirectoryIndex padmap.txt
</pre>
		<p>Now, when someone opens your PAD files containing directory in his browser by removing the filename part of any of your PAD URLs, he will see the list of all your PAD URLs instead of error 403 page.</p>
		<p class="backtotop"><a href="#protocol">Back to top</a></p>
	</div>
</div>
<div class="fbuttons"><a rel="nofollow" href="http://validator.w3.org/check?uri=referer"><img src="http://www.w3.org/Icons/valid-xhtml10-blue" style="border:0" width="88" height="31" alt="Valid XHTML 1.0 Strict" /></a> <a rel="nofollow" href="http://jigsaw.w3.org/css-validator/check/referer"><img src="http://jigsaw.w3.org/css-validator/images/vcss-blue" style="border:0" width="88" height="31" alt="Valid CSS!" /></a> <a rel="nofollow" href="http://creativecommons.org/licenses/by/3.0/"><img src="/img/cclicense.gif" width="88" height="31" style="border:0" alt="Creative Commons Attribution-Share Alike License 3.0" /></a></div>
<div class="lastmod">Last Updated: Dec 04, 2010.</div>
<div class="footer">by <a href="http://www.pad-site-scripts.com">PAD-Site-Scripts.com</a></div>
<script src="http://www.google-analytics.com/ga.js" type="text/javascript"></script>
<script type="text/javascript">
try {
var pageTracker = _gat._getTracker("UA-1483959-17");
pageTracker._trackPageview();
}
catch(err) {}
</script>
</body>
</html>