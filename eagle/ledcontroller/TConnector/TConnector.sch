<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="wirepad">
<description>&lt;b&gt;Single Pads&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="SMD1,27-2,54">
<description>&lt;b&gt;SMD PAD&lt;/b&gt;</description>
<smd name="1" x="0" y="0" dx="1.27" dy="2.54" layer="1"/>
<text x="0" y="0" size="0.0254" layer="27">&gt;VALUE</text>
<text x="-0.8" y="-2.4" size="1.27" layer="25" rot="R90">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="PAD">
<wire x1="-1.016" y1="1.016" x2="1.016" y2="-1.016" width="0.254" layer="94"/>
<wire x1="-1.016" y1="-1.016" x2="1.016" y2="1.016" width="0.254" layer="94"/>
<text x="-1.143" y="1.8542" size="1.778" layer="95">&gt;NAME</text>
<text x="-1.143" y="-3.302" size="1.778" layer="96">&gt;VALUE</text>
<pin name="P" x="2.54" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SMD2" prefix="PAD" uservalue="yes">
<description>&lt;b&gt;SMD PAD&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="PAD" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SMD1,27-2,54">
<connects>
<connect gate="1" pin="P" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="5V@1" library="wirepad" deviceset="SMD2" device="" value="5V"/>
<part name="D0" library="wirepad" deviceset="SMD2" device="" value="D0"/>
<part name="GND@1" library="wirepad" deviceset="SMD2" device="" value="GND"/>
<part name="GND@3" library="wirepad" deviceset="SMD2" device="" value="GND"/>
<part name="PAD5" library="wirepad" deviceset="SMD2" device="" value="D0"/>
<part name="5V@2" library="wirepad" deviceset="SMD2" device="" value="5V"/>
<part name="5V@3" library="wirepad" deviceset="SMD2" device="" value="5V"/>
<part name="PAD8" library="wirepad" deviceset="SMD2" device="" value="D0"/>
<part name="GND@2" library="wirepad" deviceset="SMD2" device="" value="GND"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="5V@1" gate="1" x="73.66" y="-2.54" smashed="yes" rot="R90">
<attribute name="VALUE" x="79.883" y="-1.778" size="1.778" layer="95" rot="R180"/>
</instance>
<instance part="D0" gate="1" x="71.12" y="-2.54" smashed="yes" rot="R90">
<attribute name="VALUE" x="72.517" y="-5.08" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="GND@1" gate="1" x="68.58" y="-2.54" smashed="yes" rot="R90">
<attribute name="VALUE" x="64.389" y="-1.778" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="GND@3" gate="1" x="96.52" y="22.86" smashed="yes" rot="R180">
<attribute name="VALUE" x="104.267" y="23.622" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="PAD5" gate="1" x="96.52" y="20.32" smashed="yes" rot="R180">
<attribute name="VALUE" x="102.743" y="21.082" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="5V@2" gate="1" x="96.52" y="17.78" smashed="yes" rot="R180">
<attribute name="VALUE" x="102.743" y="18.542" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="5V@3" gate="1" x="50.8" y="22.86" smashed="yes">
<attribute name="VALUE" x="44.577" y="22.098" size="1.778" layer="96"/>
</instance>
<instance part="PAD8" gate="1" x="50.8" y="20.32" smashed="yes">
<attribute name="VALUE" x="44.577" y="19.558" size="1.778" layer="96"/>
</instance>
<instance part="GND@2" gate="1" x="50.8" y="17.78" smashed="yes">
<attribute name="VALUE" x="43.307" y="17.018" size="1.778" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="D0" class="0">
<segment>
<pinref part="D0" gate="1" pin="P"/>
<wire x1="71.12" y1="0" x2="71.12" y2="20.32" width="0.1524" layer="91"/>
<pinref part="PAD5" gate="1" pin="P"/>
<wire x1="71.12" y1="20.32" x2="93.98" y2="20.32" width="0.1524" layer="91"/>
<pinref part="PAD8" gate="1" pin="P"/>
<wire x1="71.12" y1="20.32" x2="53.34" y2="20.32" width="0.1524" layer="91"/>
<junction x="71.12" y="20.32"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="GND@2" gate="1" pin="P"/>
<wire x1="53.34" y1="17.78" x2="68.58" y2="17.78" width="0.1524" layer="91"/>
<pinref part="GND@1" gate="1" pin="P"/>
<wire x1="68.58" y1="17.78" x2="68.58" y2="12.7" width="0.1524" layer="91"/>
<wire x1="68.58" y1="12.7" x2="68.58" y2="0" width="0.1524" layer="91"/>
<wire x1="68.58" y1="12.7" x2="76.2" y2="12.7" width="0.1524" layer="91"/>
<wire x1="76.2" y1="12.7" x2="76.2" y2="22.86" width="0.1524" layer="91"/>
<pinref part="GND@3" gate="1" pin="P"/>
<wire x1="76.2" y1="22.86" x2="93.98" y2="22.86" width="0.1524" layer="91"/>
<junction x="68.58" y="12.7"/>
</segment>
</net>
<net name="5V" class="0">
<segment>
<pinref part="5V@1" gate="1" pin="P"/>
<wire x1="73.66" y1="0" x2="73.66" y2="17.78" width="0.1524" layer="91"/>
<pinref part="5V@2" gate="1" pin="P"/>
<wire x1="93.98" y1="17.78" x2="73.66" y2="17.78" width="0.1524" layer="91"/>
<wire x1="73.66" y1="17.78" x2="73.66" y2="22.86" width="0.1524" layer="91"/>
<pinref part="5V@3" gate="1" pin="P"/>
<wire x1="73.66" y1="22.86" x2="53.34" y2="22.86" width="0.1524" layer="91"/>
<junction x="73.66" y="17.78"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
