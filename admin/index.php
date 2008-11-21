<?PHP

	//////////////////////////////////////////////////
	//		 	 ____    __           ______       			//
	//			/\  _`\ /\ \__       /\  _  \      			//
	//			\ \ \L\_\ \ ,_\   ___\ \ \L\ \     			//
	//			 \ \  _\L\ \ \/  / __`\ \  __ \    			//
	//			  \ \ \L\ \ \ \_/\ \L\ \ \ \/\ \   			//
	//	  		 \ \____/\ \__\ \____/\ \_\ \_\  			//
	//			    \/___/  \/__/\/___/  \/_/\/_/  	 		//
	//																					 		//
	//////////////////////////////////////////////////
	// The Andromeda-Project-Browsergame				 		//
	// Ein Massive-Multiplayer-Online-Spiel			 		//
	// Programmiert von Nicolas Perrenoud				 		//
	// www.nicu.ch | mail@nicu.ch								 		//
	// als Maturaarbeit '04 am Gymnasium Oberaargau	//
	//////////////////////////////////////////////////
	//
	// 	Dateiname: index.php
	// 	Topic: Admin Index
	// 	Autor: Nicolas Perrenoud alias MrCage
	// 	Erstellt: 01.12.2004
	// 	Bearbeitet von: Nicolas Perrenoud alias MrCage
	// 	Bearbeitet am: 23.04.2006
	// 	Kommentar: 	Layout und generelle Definitionen f� Admin-Modus
	//

	require("inc/includer.inc.php");

	// Zwischenablage
	if (isset($_GET['cbclose']))
	{
		$_SESSION['clipboard'] = null;
	}
	$cb = isset ($_SESSION['clipboard']) && $_SESSION['clipboard']==1 ? true : false;



			
	adminHtmlHeader($s['theme']);
			
			
			
	// Admin-Gruppen laden				
	$admingroup=array();
	$gres=dbquery("SELECT * FROM admin_groups ORDER BY group_level DESC;");
	while ($garr=mysql_fetch_array($gres))
	{
		$admingroup[$garr['group_id']] =$garr['group_name'];
		$adminlevel[$garr['group_level']] =$garr['group_name'];
	}

				?>

				<!-- Stuff for DHTML Tipps -->
				<div id="Migoicons" style="visibility:hidden;position:absolute;z-index:1000;top:0;border:none"></div>
				<script  type="text/javascript">
					stl=["white","##222255","","","",,"white","#606578","","","",,,,2,"#222255",2,,,,,"",,,,]
					var TipId="Migoicons"
					var FiltersEnabled = 1
					mig_clay()
				</script>

				<table id="layoutbox">
					<tr>
						<td id="topbar" colspan="3">
							<?PHP
								foreach ($topnav as $title=> $data)
								{
									echo " &nbsp; <a href=\"".$data['url']."\" onclick=\"window.open('".$data['url']."');return false;\">$title</a>";
								}
								echo "<br/>";								
							
								echo '<a href="?adminlist=1" style="color:#0f0;">Adminliste</a> | ';
								echo '<a href="?myprofile=1" style="color:#0f0;">Mein Profil</a> | ';
								echo "<a href=\"javascript:;\" style=\"color:#f90;\" onclick=\"window.open('popup.php?page=notepad','Notepad','width=600, height=500, status=no, scrollbars=yes')\">";
								echo "Notizblock";
								$nres = dbquery("select COUNT(*) from admin_notes where admin_id='".$s['user_id']."'");
								$narr = mysql_fetch_row($nres);
								if ($narr[0]>0)
								{
									echo " (".$narr[0].")";
								}
								echo "</a> | ";
								echo "<a href=\"javascript:;\" style=\"color:#f90;\" onclick=\"window.open('popup.php?page=tickets','Tickets','width=700, height=600, status=no, scrollbars=yes')\">";
								echo "Tickets";
								$nres = dbquery("select COUNT(*) from tickets where status=0");
								$narr = mysql_fetch_row($nres);
								if ($narr[0]>0)
								{
									echo " (".$narr[0].")";
								}
								echo "</a> | ";
								if (!$cb)
								{
									echo "<a href=\"frameset.php?page=$page&amp;sub=$sub\" target=\"_top\" style=\"color:#ff0;\">Zwischenablage</a> | ";
								}
								echo '<a href="?logout=1" style="color:#f90;">Logout</a>';
							?>
						</td>
					</tr>
					<tr>
						<td id="logo">&nbsp;</td>
						<td id="banner" colspan="2"><?PHP echo GAMEROUND_NAME;?></td>
					</tr>
					<tr>
						<td id="menu1">
							<?php
							
								//
								// Linke Navigation anzeigen
								//
								foreach ($navmenu as $cat=> $item)
								{
									$nitem = array_values($item);
									echo "<a href=\"?page=".$nitem[0]['page']."\" class=\"menu1Title\">$cat</a>";
									if ($nitem[0]['page']==$page)
									{
										foreach ($item as $title=> $data)
										{
											if ($title=="bar")
											{
												echo "<hr noshade=\"noshade\" size=\"1\" style=\"background:#fff;margin:0px 20px 0px 20px;\" />";
											}
											else
											{
												if ($data['level']<=$_SESSION[SESSION_NAME]['group_level'])
												{
													if ($data['sub']!="")
													{
														echo "<a href=\"?page=".$data['page']."&amp;sub=".$data['sub']."\" class=\"menu1Item\" ";
														if (isset($data['new']) && $data['new']==1)
															echo " style=\"color:#ff0;\"";
														echo ">";
														if ($page==$data['page'] && $sub==$data['sub'])
															echo "<b>&gt;</b> ";
														echo "$title </a>";
													}
													else
													{
														echo "<a href=\"?page=".$data['page']."\" class=\"menu1Item\" >";
														if ($page==$data['page'] && $sub=="")
															echo "<b>&gt;</b> ";
														echo "$title</a>";
													}
												}
											}
										}
									}
								}

								echo "<br/>
								<form action=\"?page=user&amp;action=search\" method=\"post\">
								<div>
										 &nbsp;<input type=\"text\" name=\"user_nick_search\" size=\"9\" autocomplete=\"off\" onkeyup=\"\" id=\"user_search_box1\" />
										<input type=\"hidden\" name=\"qmode[user_nick_search]\" value=\"LIKE '%\" />
										<input type=\"submit\" name=\"user_search\" value=\"Usersuche\" />
									</div></form>";

								//
								// Auslastung
								//
								$g_style=" style=\"color:#0f0\"";
								$y_style=" style=\"color:#ff0\"";
								$o_style=" style=\"color:#fa0\"";
								$r_style=" style=\"color:#f55\"";

								$ures=dbquery("SELECT count(*) FROM users;");
								$uarr=mysql_fetch_row($ures);
								$up=$uarr[0]/$conf['enable_register']['p2'];
								$p1res=dbquery("SELECT count(*) FROM planets WHERE planet_user_id>0;");
								$p1arr=mysql_fetch_row($p1res);
								$p2res=dbquery("SELECT count(*) FROM planets;");
								$p2arr=mysql_fetch_row($p2res);
								if ($p2arr[0]>0)
									$pp=$p1arr[0]/$p2arr[0];
								else
									$pp=0;
								$s1res=dbquery("SELECT count(entities.cell_id) FROM entities,planets WHERE planets.id=entities.id AND planet_user_id>0 GROUP BY entities.cell_id;");
								$s1arr=mysql_num_rows($s1res);
								$s2res=dbquery("SELECT count(*) FROM entities WHERE code='s';");
								$s2arr=mysql_fetch_row($s2res);
								if ($s2arr[0]>0)
									$sp=$s1arr/$s2arr[0];
								else
									$sp=0;
								echo "<br/><div class=\"menutitle\">Auslastung:</div>";
								echo "<table class=\"tb\">";
								echo "<tr><th>User:</th>";
								if ($up<0.5) $tbs=$g_style;
								elseif ($up<0.8) $tbs=$y_style;
								elseif ($up<0.9) $tbs=$o_style;
								else $tbs=$r_style;
								echo "<td $tbs>".$uarr[0]." / ".$conf['enable_register']['p2']."</td><td $tbs>".round($up*100,1)."%</td></tr>";
								echo "<tr><th>Planeten:</th>";
								if ($pp<0.5) $tbs=$g_style;
								elseif ($pp<0.8) $tbs=$y_style;
								elseif ($pp<0.9) $tbs=$o_style;
								else $tbs=$r_style;
								echo "<td $tbs>".$p1arr[0]." / ".$p2arr[0]."</td><td $tbs>".round($pp*100,1)."%</td></tr>";
								echo "<tr><th>Systeme:</th> ";
								if ($sp<0.5) $tbs=$g_style;
								elseif ($sp<0.8) $tbs=$y_style;
								elseif ($sp<0.9) $tbs=$o_style;
								else $tbs=$r_style;
								echo "<td $tbs>".$s1arr." / ".$s2arr[0]."</td><td $tbs>".round($sp*100,1)."%</td></tr>";
								echo "</table>";

								// Online
								echo "<br/><div class=\"menutitle\">Online:</div>";
								$gres=dbquery("SELECT COUNT(*) FROM users WHERE user_acttime>".(time()-$conf['user_timeout']['v']).";");
								$garr=mysql_fetch_row($gres);
								if ($uarr[0]>0)
									$gp=$garr[0]/$uarr[0]*100;
								else
									$gp=0;
								$a1res=dbquery("SELECT COUNT(*)  FROM admin_users WHERE user_acttime>".(time()-TIMEOUT)." AND user_session_key!='';");
								$a1arr=mysql_fetch_row($a1res);
								$a2res=dbquery("SELECT COUNT(*)  FROM admin_users;");
								$a2arr=mysql_fetch_row($a2res);
								if ($a2arr[0]>0)
									$ap=$a1arr[0]/$a2arr[0]*100;
								else
									$ap=0;
								echo "<table class=\"tb\">";
								echo "<tr><th><a href=\"?page=user&amp;sub=userlog\">User:</a></th><td>".$garr[0]." / ".$uarr[0]."</td><td>".round($gp,1)."%</td></tr>";
								echo "<tr><th><a href=\"?page=home&amp;sub=adminlog\">Admins:</a></th><td>".$a1arr[0]." / ".$a2arr[0]."</td><td>".round($ap,1)."%</td></tr>";
								echo "</table><br/>";
								echo "<div style=\"padding-left:10px;\">
								<b>PHP:</b> ".substr(phpversion(),0,10)."<br/>
								<b>MySQL:</b> ".mysql_get_client_info()."<br/>
								<b>Webserver:</b> ".apache_get_version()."
								</div>";
							?>
						</td>
						<td id="content">
							<?php
								// Inhalt einbinden

								if (isset($_GET['adminlist']))
								{									
									require("inc/adminlist.inc.php");
								}
								elseif (isset($_GET['myprofile']))
								{									
									require("inc/myprofile.inc.php");
								}
								else
								{
									// Release update lock
									if (isset($_GET['releaseupdate']) && $_GET['releaseupdate']==1)
									{
										dbquery("UPDATE config SET config_value=0 WHERE config_name='updating';");
									}
									
									// Release fleet update lock  
									if (isset($_GET['releasefleetupdate']) && $_GET['releasefleetupdate']==1)
									{
										dbquery("UPDATE config SET config_value=0 WHERE config_name='updating_fleet';");
									}
									
									// Activate update system
									if (isset($_GET['activateupdate']) && $_GET['activateupdate']==1)
									{
										dbquery("UPDATE config SET config_value=1 WHERE config_name='update_enabled';");
									}							
									
									if ($conf['updating']['v']!=0 && ($conf['updating']['p2']=="" || $conf['updating']['p2']<time()-120))
									{
										echo "<br/>";
										iBoxStart("Update-Problem");
										echo "Das Update k&ouml;nnte unter Umst&auml;nden festh&auml;ngen.";
										if ($conf['updating']['p2']>0)
											echo "Es wurde um ".date("d.m.Y, H:i",$conf['updating']['p2'])." zuletzt ausgeführt";
										echo " <a href=\"?page=$page&amp;releaseupdate=1\">L&ouml;sen</a>";
										iBoxEnd();
									}
									if ($conf['updating_fleet']['v']!=0 && ($conf['updating_fleet']['p2']=="" || $conf['updating_fleet']['p2']<time()-120))
									{
										echo "<br/>";
										iBoxStart("Flottenupdate-Problem");
										echo "Das Flottenupdate k&ouml;nnte unter Umst&auml;nden festh&auml;ngen.";
										if ($conf['updating_fleet']['p2']>0)
											echo "Es wurde um ".date("d.m.Y, H:i",$conf['updating_fleet']['p2'])." zuletzt ausgefhrt";
										echo " <a href=\"?page=$page&amp;releasefleetupdate=1\">L&ouml;sen</a>";
										iBoxEnd();
									}
									if ($conf['update_enabled']['v']!=1)
									{
										echo "<br/>";
										iBoxStart("Updates deaktiviert");
										echo "Die Updates sind momentan deaktiviert!";
										echo " <a href=\"?page=$page&amp;activateupdate=1\">Aktivieren</a>";
										iBoxEnd();
									}
									
									$allow_inc=false;
									$rank="";
									foreach ($navmenu as $cat=> $item)
									{
										foreach ($item as $title=> $data)
										{
											if ($title != "bar" && $data['page']==$page && $data['sub']==$sub)
											{
												$rank=$data['level'];
												if ($data['level']<=$_SESSION[SESSION_NAME]['group_level'])
													$allow_inc=true;
											}
										}
									}
									if ($allow_inc || $rank=="")
									{
										if (eregi("^[a-z\_]+$",$page)  && strlen($page)<=50)
										{
											if (!include("content/".$page.".php"))
												cms_err_msg("Die Seite $page wurde nicht gefunden!");
										}
										else
											echo "<h1>Fehler</h1>Der Seitenname <b>".$page."</b> enth&auml;lt unerlaubte Zeichen!<br><br><a href=\"javascript:history.back();\">Zur&uuml;ck</a>";
									}
									else
										echo "<h1>Kein Zugriff</h1> Du hast keinen Zugriff auf diese Seite!<br/><br/> Erwartet: <b>".$adminlevel[$rank]." ($rank)</b>, du bist <b>".$_SESSION[SESSION_NAME]['group_name']." (".$_SESSION[SESSION_NAME]['group_level'].")</b>.";
								}
							?>
						</td>
					</tr>
					<tr>
						<td id="copy">
							&copy;<?PHP echo date('Y');?> by etoa.ch
						</td>
						<td id="bottombar" colspan="2">
							<?php
								echo "<b>Zeit: </b>".date("H:i:s")." &nbsp; ";
								// Renderzeit
								$render_time = explode(" ",microtime());
								$rtime = $render_time[1]+$render_time[0]-$render_starttime;
								echo "<b>Renderzeit:</b> ".round($rtime,3)." sec &nbsp; ";
								// Nickname
								echo "<b>Eingeloggt als: </b>".$_SESSION[SESSION_NAME]['user_nick']." &nbsp; ";
							?>
						</td>
					</tr>
				</table>
				
	<?PHP
		adminHtmlFooter();			
		
	require("inc/footer.inc.php");
?>


