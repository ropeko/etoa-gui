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
	// 	Dateiname: admin_cunftions.inc.php
	// 	Topic: Funktionen für Formularverwaltung und Admin-Modus
	// 	Autor: Nicolas Perrenoud alias MrCage
	// 	Erstellt: 01.12.2004
	// 	Bearbeitet von: Nicolas Perrenoud alias MrCage
	// 	Bearbeitet am: 04.07.2007
	// 	Kommentar:
	//

	function adminHtmlHeader($themePath = "default.css")
	{
		global $conf,$xajax;		
		if (!is_file("themes/".$themePath))
			$themePath = "default.css";
		echo '<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">	
	<head>
		<title>'.$conf['game_name']['v'].' '.$conf['game_name']['p1'].' Administration - '.GAMEROUND_NAME.'</title>
		<link rel="stylesheet" type="text/css" href="themes/'.$themePath.'" />
		<link rel="stylesheet" href="../css/general.css" type="text/css" />

		<meta name="author" content="Nicolas Perrenoud" />
		<meta name="keywords" content="Escape to Andromeda, Browsergame, Strategie, Simulation, Andromeda, MMPOG, RPG" />
		<meta name="robots" content="nofollow" />

		<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
		<meta http-equiv="Content-Script-Type" content="text/javascript" />
		<meta http-equiv="Content-Style-Type" content="text/css" />
		<meta http-equiv="content-language" content="de" />

		<script src="../js/main.js" type="text/javascript"></script>
		<script src="../js/admin.js" type="text/javascript"></script>
		<script src="../js/tooltip.js" type="text/javascript" ></script>
		';
		$xajax->printJavascript("../".XAJAX_DIR); 

	echo '	</head>
	<body class="index">';
	initTT();
		
	}
	
	function adminHtmlFooter()
	{
		echo '	</body>
</html>';
	}

	/**
	* Shows a table view of a given mysql result
	*
	* @param string MySQL result pointer
	*/	
	function db_show_result($res)
	{
		echo '<table class="tb"><tr>';
		$fc=0;
		while ($fo = mysql_fetch_field($res))
		{
			echo '<th>'.$fo->name.'</th>';
			$fc++;
		}		
		echo '</tr>';
		while ($arr=mysql_fetch_row($res))
		{
			echo '<tr>';
			for ($x=0;$x<$fc;$x++)
			{
				echo '<td>'.$arr[$x].'</td>';
			}
			echo '</tr>';
		}
		echo '</table>';		
	}

	/**
	* Shows a formated error message
	*
	* @param string Message
	*/
	function cms_err_msg($msg,$id="errmsg")
	{
		echo "<div class=\"errorBox\" id=\"$id\"><b>Fehler:</b> ".text2html($msg)."</div>";
	}

	/**
	* Shows a success error message
	*
	* @param string Message
	*/
	function cms_ok_msg($msg,$id="okmsg")
	{
		echo "<div class=\"successBox\" id=\"$id\">".text2html($msg)."</div>";
	}

	/**
	* Generates a page for editing table date with
	* an advanced form
	*
	* @param string Module-key
	*/
	function advanced_form($module)
	{
		require_once("inc/form_functions.php");
		require_once("forms/$module.php");
		require_once("inc/advanced_forms.php");
	}

	/**
	* Generates a page for editing table date with
	* a simple form
	*
	* @param string Module-key
	*/
	function simple_form($module)
	{
		require_once("inc/form_functions.php");
		require_once("forms/$module.php");
		require_once("inc/simple_forms.php");
	}

	/**
	* Checks permission to access a page
	* for current user and given page rank
	*
	* @param int Required rank
	* @return bool Permission granted or nor
	*/
	function check_perm($rank)
	{
		global $adminlevel;
		if ($_SESSION[SESSION_NAME]['group_level']<$rank)
		{
			echo "<h1>Kein Zugriff</h1> Du hast keinen Zugriff auf diese Seite! Erwartet ".$adminlevel[$rank]." ($rank), gegeben ".$_SESSION[SESSION_NAME]['group_name']." (".$_SESSION[SESSION_NAME]['group_level'].")<br/>";
			return false;
		}
		return true;
	}

	/**
	* Displays a select box for choosing the search method 
	* for varchar/text mysql table fields ('contains', 'part of' 
	* and negotiations of those two)
	*
	* @param string Field name
	*/
	function fieldqueryselbox($name)
	{
		echo "<select name=\"qmode[$name]\">";
		echo "<option value=\"LIKE '%\">enth&auml;lt</option>";
		echo "<option value=\"LIKE '\">ist gleich</option>";
		echo "<option value=\"NOT LIKE '%\">enth&auml;lt nicht</option>";
		echo "<option value=\"NOT LIKE '\">ist ungleich</option>";
		echo "<option value=\"< '\">ist kleiner</option>";
		echo "<option value=\"> '\">ist gr&ouml;sser</option>";
		echo "</select>";
	}
	/**
	* Displays a select box for choosing the search method 
	* for varchar/text mysql table fields ('contains', 'part of' 
	* and negotiations of those two)
	*
	* @param string Field name
	*/
	function searchFieldTextOptions($name)
	{
		ob_start();
		echo "<select name=\"qmode[$name]\">";
		echo "<option value=\"%\">enth&auml;lt</option>";
		echo "<option value=\"!%\">enth&auml;lt nicht</option>";
		echo "<option value=\"=\">ist gleich</option>";
		echo "<option value=\"!=\">ist ungleich</option>";
		echo "</select>";
		$res = ob_get_contents();
		ob_end_clean();
		return $res;
	}	

	/**
	* Displays a select box for choosing the search method 
	* for varchar/text mysql table fields ('contains', 'part of' 
	* and negotiations of those two)
	*
	* @param string Field name
	*/
	function searchFieldNumberOptions($name)
	{
		ob_start();
		echo "<select name=\"qmode[$name]\">";
		echo "<option value=\"=\">=</option>";
		echo "<option value=\"!=\">!=</option>";
		echo "<option value=\"<\">&lt;</option>";
		echo "<option value=\"<=\">&lt;=</option>";
		echo "<option value=\">\">&gt;</option>";
		echo "<option value=\">=\">&gt;=</option>";
		echo "</select>";
		$res = ob_get_contents();
		ob_end_clean();
		return $res;
	}	
	
	function searchFieldOptionsName($opt='')
	{
		switch ($opt)
		{
			case "=":
				return "gleich";			
			case "!=":
				return "ungleich";			
			case "%":
				return "enthält";			
			case "!%":
				return "enthält nicht";			
			case "<":
				return "kleiner als";			
			case "<=":
				return "kleiner gleich";			
			case ">":
				return "grösser als";			
			case ">=":
				return "grösser gleich";			
			default:
				return "gleich";			
		
		}
	}
	
	function searchFielsOptionsSql($value,$opt)
	{
		switch ($opt)
		{
			case "=":
				return " LIKE '".$value."' ";			
			case "!=":
				return " NOT LIKE '".$value."' ";			
			case "%":
				return " LIKE '%".$value."%' ";			
			case "!%":
				return " NOT LIKE '%".$value."%' ";			
			case "<":
				return " < ".intval($value)." ";			
			case "<=":
				return " <= ".intval($value)." ";			
			case ">":
				return " > ".intval($value)." ";			
			case ">=":
				return " >= ".intval($value)." ";			
			default:
				return " ='".$value."'";			
		}		
	}
	
	


	/**
	* Displays a clickable edit button
	*
	* @param string Url of the link
	* @param string Optional onclick value
	*/
	function edit_button($url, $ocl="")
	{
		if ($ocl!="")
			return "<a href=\"$url\" onclick=\"$ocl\"><img src=\"../images/edit.gif\" alt=\"Bearbeiten\" style=\"width:16px;height:18px;border:none;\" title=\"Bearbeiten\" /></a>";
		else
			return "<a href=\"$url\"><img src=\"../images/edit.gif\" alt=\"Bearbeiten\" style=\"width:16px;height:18px;border:none;\" title=\"Bearbeiten\" /></a>";
	}
	
	/**
	* Displays a clickable edit button
	*
	* @param string Url of the link
	* @param string Optional onclick value
	*/
	function cb_button($url)
	{
		global $cb;
		if ($cb)
		{
			return "<a href=\"clipboard.php?".$url."\" target=\"clipboard\"><img src=\"../images/clipboard.png\" alt=\"Zwischenablage\" style=\"width:16px;height:18px;border:none;\" title=\"Zwischenablage\" /></a>";
		}
		return "";
	}	
	

	/**
	* Displays a clickable repair button
	*
	* @param string Url of the link
	* @param string Optional onclick value
	*/
	function repair_button($url, $tmTitle="", $tmText="")
	{
		if ($tmTitle!="" && $tmText!="")
			return "<a href=\"$url\" onclick=\"$ocl\"><img src=\"../images/repair.gif\" alt=\"Reparieren\" style=\"width:18px;height:18px;border:none;\" ".tm($tmTitle,$tmText)."/></a>";
		else
			return "<a href=\"$url\"><img src=\"../images/repair.gif\" alt=\"Reparieren\" style=\"width:18px;height:18px;border:none;\" title=\"Reparieren\" /></a>";
	}

	/**
	* Displays a clickable delete button
	*
	* @param string Url of the link
	* @param string Optional onclick value
	*/
	function del_button($url, $ocl="")
	{
		if ($ocl!="")
			return "<a href=\"$url\" onclick=\"$ocl\"><img src=\"../images/delete.gif\" alt=\"Löschen\" style=\"width:16px;height:15px;border:none;\" title=\"Löschen\" /></a>";
		else
			return "<a href=\"$url\"><img src=\"../images/delete.gif\" alt=\"Löschen\" style=\"width:18px;height:15px;border:none;\" title=\"Löschen\" /></a>";
	}


	/**
	* Displays a tool for securing a directory with a
	* .tpasswd and .htaccess file
	*
	* @param string Default AuthName
	* @param string Default user nick
	* @param string Default directory to store .htpasswd file
	*/
	function htaccess_tool($name,$user,$dir)
	{
		echo "<h2>Passwort-Schutz für Admin-Modus</h2>";
		if ($_POST['htaccess_submit']!="")
		{
			if ($_POST['htaccess_name']!="" && $_POST['htaccess_file']!="" && $_POST['htaccess_user']!="" && $_POST['htaccess_password']!="")
			{
				$f=fopen($dir."/".HTACCESS_FILE,"w+");
				fwrite($f,"AuthType Basic\n");
				fwrite($f,"AuthName \"".$_POST['htaccess_name']."\"\n");
				fwrite($f,"AuthUserFile ".$_POST['htaccess_file']."\n");
				fwrite($f,"require valid-user");
				fclose($f);
				passthru(HTPASSWD_COMMAND." -bc ".$_POST['htaccess_file']." ".$_POST['htaccess_user']." ".$_POST['htaccess_password']);
				echo "Passwortdatei erstellt!<br/><br/>";
			}
			else
				echo "<b>Fehler!</b> Eines oder mehrere Felder sind nicht ausgefüllt!<br/><br/>";
		}

		$file = $dir."/".HTPASSWD_FILE;
		$active=false;
		if (file_exists($dir."/".HTACCESS_FILE))
		{
			$f = fopen($dir."/".HTACCESS_FILE,"r");
			while ($t=fgets($f,500))
			{
				$a = explode(" ",$t);
				if ($a[0]=="AuthName") $name=substr($a[1],1,strlen($a[1])-2);
				if ($a[0]=="AuthUserFile") $file=substr($a[1],0,strlen($a[1])-1);
			}
			fclose($f);
			if (file_exists($file))
			{
				$f = fopen($file,"r");
				while ($t=fgets($f,500))
				{
					$a = explode(":",$t);
					$user=$a[0];
				}
				fclose($f);
				$active=true;
			}
			else
				echo "<b>Fehler:</b>Definitionsdatei gefunden aber keine Passwortdatei ($file)!<br/><br/>";
		}

		if ($_POST['htaccess_remove']!="" && $active)
		{
			unlink($dir."/".HTACCESS_FILE);
			unlink($file);
			$active=false;
		}

		if ($active)
			echo "<div style=\"color:#0f0\">Der Passwortschutz ist aktiv!</div><br/>";
		else
			echo "<div style=\"color:#f00\">Der Passwortschutz ist NICHT aktiv!</div><br/>";

		echo "<form action=\"\" method=\"post\">";
		echo "<table class=\"tb\">";
		echo "<tr><th>Zonenname:</th><td><input type=\"text\" value=\"".$name."\" name=\"htaccess_name\" size=\"40\" /></td></tr>";
		echo "<tr><th>Name der Passwortdatei:</th><td><input type=\"text\" value=\"".$file."\" name=\"htaccess_file\" size=\"50\" /></td></tr>";
		echo "<tr><th>User:</th><td><input type=\"text\" value=\"".$user."\" name=\"htaccess_user\" size=\"40\" /></td></tr>";
		echo "<tr><th>Passwort	:</th><td><input type=\"text\" value=\"\" name=\"htaccess_password\" size=\"40\" /></td></tr>";
		echo "</table><br/><br/><input type=\"submit\" value=\"Speichern\" name=\"htaccess_submit\" />";
		if ($active)
			echo " <input type=\"submit\" value=\"Deaktivieren\" name=\"htaccess_remove\" />";
		echo "</form>";
	}


	/**
	* Displays a tool for securing a directory with a
	* .tpasswd and .htaccess file
	*
	* @param string Default AuthName
	* @param string Default user nick
	* @param string Default directory to store .htpasswd file
	*/
	function htpasswd_tool($user,$file)
	{
		$file = getcwd()."/../".$file;

		echo "<h2>Passwort-Schutz für Admin-Modus</h2>";
		
		if (file_exists($file))
		{
			$userarr = posix_getpwuid(fileowner($file));
			$user = $userarr['name'];
			if ($user==UNIX_USER)
			{
	    		$userarr = posix_getpwuid(filegroup($file));
	    		$user = $userarr['name'];
	    		if ($user==UNIX_GROUP)
	    		{		    			
	    			$perms = substr(sprintf("%o",fileperms($file)),2,3);
						if (substr($perms,1,1)>=6)
						{
							if ($_POST['htaccess_submit']!="")
							{
								if ($_POST['htaccess_user']!="" && $_POST['htaccess_password']!="")
								{
									$cmd = HTPASSWD_COMMAND." -bc ".$file." ".$_POST['htaccess_user']." ".$_POST['htaccess_password'];
									passthru($cmd);
									echo "Passwortdatei erstellt!<br/><br/>";
								}
								else
								{
									echo "<b>Fehler!</b> Eines oder mehrere Felder sind nicht ausgefüllt!<br/><br/>";
								}
							}
					
							$active=false;
							
							if (file_exists($file))
							{
								$f = fopen($file,"r");
								while ($t=fgets($f,500))
								{
									$a = explode(":",$t);
									$user=$a[0];
									$pw=$a[1];
								}
								fclose($f);
							}
							else
								echo "<b>Fehler:</b>Keine Passwortdatei gefunden ($file)!<br/><br/>";
					
							echo "<form action=\"\" method=\"post\">";
							echo "<table class=\"tb\">";
							echo "<tr><th>User:</th><td><input type=\"text\" value=\"".$user."\" name=\"htaccess_user\" size=\"40\" /></td></tr>";
							echo "<tr><th>Neues Passwort:</th><td><input type=\"text\" value=\"\" name=\"htaccess_password\" size=\"40\" /></td></tr>";
							echo "</table><br/><br/><input type=\"submit\" value=\"Speichern\" name=\"htaccess_submit\" />";
							if ($active)
							{
								echo " <input type=\"submit\" value=\"Deaktivieren\" name=\"htaccess_remove\" />";
							}
							echo "</form>";


						}
						else
						{
							error_msg("Die Passwortdatei [b]".$file."[/b] hat falsche Gruppenrechte ($perms)!\nDies kann mit [i]chmod g+w ".$file." -R[/i] geändert werden!");
						}
	    		}
	    		else
	    		{
						error_msg("Die Passwortdatei [b]".$file."[/b] hat eine falsche Gruppe! Eingestellt ist [b]".$user."[/b], erwartet wurde [b]".UNIX_GROUP."[/b]!\nDies kann mit [i]chgrp ".UNIX_GROUP." ".$file." -R[/i] geändert werden!");
	    		}
			}
			else
			{
				error_msg("Die Passwortdatei [b]".$file."[/b] hat einen falschen Besitzer! Eingestellt ist [b]".$user."[/b], erwartet wurde [b]".UNIX_USER."[/b]!\nDies kann mit [i]chown ".UNIX_USER." ".$file." -R[/i] geändert werden!");
			}
		}
		else
		{
			error_msg("Die Passwortdatei [b]".$file."[/b] wurde nicht gefunden!");
		}
	}
	
	function htpasswd_check($user,$file)
	{
		$file = getcwd()."/../".$file;
		if (!file_exists($file))
		{
			passthru(HTPASSWD_COMMAND." -bc ".$file." $user \"\"");
			return false;
		}
		else
		{
			return true;
		}
	}	

	function display_field($type,$confname,$field)
	{
		global $conf;
		switch ($type)
		{
			case "text":
				echo "<input type=\"text\" name=\"config_".$field."[".$confname."]\" value=\"".$conf[$confname][$field]."\" />";
				break;
			case "textarea":
				echo "<textarea name=\"config_".$field."[".$confname."]\" rows=\"4\" cols=\"50\">".$conf[$confname][$field]."</textarea>";
				break;
			case "onoff":
				echo "Ja: <input type=\"radio\" name=\"config_".$field."[".$confname."]\" value=\"1\" ";
				if ($conf[$confname][$field]==1) echo " checked=\"checked\"";
				echo " /> Nein: <input type=\"radio\" name=\"config_".$field."[".$confname."]\" value=\"0\" ";
				if ($conf[$confname][$field]==0) echo " checked=\"checked\"";
				echo " />";
				break;
			case "timedate":
				echo "<select name=\"config_".$field."_d[".$confname."]\">";
				for ($x=1;$x<32;$x++)
				{
					echo "<option value=\"$x\"";
					if (date("d",$conf[$confname][$field])==$x) echo " selected=\"selected\"";
					echo ">";
					if ($x<10) echo 0;
					echo "$x</option>";
				}
				echo "</select>.";
				echo "<select name=\"config_".$field."_m[".$confname."]\">";
				for ($x=1;$x<32;$x++)
				{
					echo "<option value=\"$x\"";
					if (date("m",$conf[$confname][$field])==$x) echo " selected=\"selected\"";
					echo ">";
					if ($x<10) echo 0;
					echo "$x</option>";
				}
				echo "</select>.";
				echo "<select name=\"config_".$field."_y[".$confname."]\">";
				for ($x=date("Y")-50;$x<date("Y")+50;$x++)
				{
					echo "<option value=\"$x\"";
					if (date("Y",$conf[$confname][$field])==$x) echo " selected=\"selected\"";
					echo ">$x</option>";
				}
				echo "</select> ";
				echo "<select name=\"config_".$field."_h[".$confname."]\">";
				for ($x=0;$x<25;$x++)
				{
					echo "<option value=\"$x\"";
					if (date("H",$conf[$confname][$field])==$x) echo " selected=\"selected\"";
					echo ">";
					if ($x<10) echo 0;
					echo "$x</option>";
				}
				echo "</select>:";
				echo "<select name=\"config_".$field."_i[".$confname."]\">";
				for ($x=0;$x<60;$x++)
				{
					echo "<option value=\"$x\"";
					if (date("i",$conf[$confname][$field])==$x) echo " selected=\"selected\"";
					echo ">";
					if ($x<10) echo 0;
					echo "$x</option>";
				}
				echo "</select>:";
				echo "<select name=\"config_".$field."_s[".$confname."]\">";
				for ($x=0;$x<60;$x++)
				{
					echo "<option value=\"$x\"";
					if (date("s",$conf[$confname][$field])==$x) echo " selected=\"selected\"";
					echo ">";
					if ($x<10) echo 0;
					echo "$x</option>";
				}
				echo "</select>";
		}
	}

	function create_sql_value($type,$confname,$field,$postarray)
	{
		global $conf;
		switch ($type)
		{
			case "text":
				$sql_value = $postarray['config_'.$field][$confname];
				break;
			case "textarea":
				$sql_value = $postarray['config_'.$field][$confname];
				break;
			case "onoff":
				$sql_value = $postarray['config_'.$field][$confname];
				break;
			case "timedate":
				$sql_value = mktime($postarray['config_'.$field.'_h'][$confname],$postarray['config_'.$field.'_i'][$confname],$postarray['config_'.$field.'_s'][$confname],$postarray['config_'.$field.'_m'][$confname],$postarray['config_'.$field.'_d'][$confname],$postarray['config_'.$field.'_y'][$confname]);
				break;
		}
		return $sql_value;
	}

	// Wandelt den Text in brauchbare HTML um
	function encode_logtext($string)
	{
		$string = eregi_replace('\[USER_ID=([0-9]*);USER_NICK=([^\[]*)\]', '<a href="?page=user&sub=edit&user_id=\1">\2</a>', $string);
		$string = eregi_replace('\[PLANET_ID=([0-9]*);PLANET_NAME=([^\[]*)\]', '<a href="?page=galaxy&sub=edit&planet_id=\1">\2</a>', $string);
		
		return $string;
	}


	function searchQuery($data)
	{
		foreach ($data as $k=>$v)
		{
			if(!isset($str))
			{
				$str=base64_encode($k).":".base64_encode($v);
			}
			else
			{
				$str.=";".base64_encode($k).":".base64_encode($v);
			}
		}
		return base64_encode($str);
	}
	
	function searchQueryDecode($query)
	{
		$str = explode(";",base64_decode($query));
		$res = array();
		foreach ($str as $s)
		{
			$t = explode(":",$s);
			$res[base64_decode($t[0])]=base64_decode($t[1]);
		}
		return $res;
	}


	function calcBuildingPoints($id=0)
	{
		$cfg = Config::getInstance();
		if ($id>0)
			$sql = "SELECT
				building_id,
	      building_costs_metal,
	      building_costs_crystal,
	      building_costs_fuel,
	      building_costs_plastic,
	      building_costs_food,
				building_build_costs_factor,
	      building_last_level
			FROM
				buildings
			WHERE
				building_id=".$id.";";
		else	
			$sql = "SELECT
				building_id,
	      building_costs_metal,
	      building_costs_crystal,
	      building_costs_fuel,
	      building_costs_plastic,
	      building_costs_food,
				building_build_costs_factor,
	      building_last_level
			FROM
				buildings;";
			dbquery("DELETE FROM building_points;");
			$res = dbquery($sql);
			$mnr = mysql_num_rows($res);
			if ($mnr>0)
			{
				while ($arr = mysql_fetch_array($res))
				{
					for ($level=1;$level<=intval($arr['building_last_level']);$level++)
					{
						$r = $arr['building_costs_metal']
						+$arr['building_costs_crystal']
						+$arr['building_costs_fuel']
						+$arr['building_costs_plastic']
						+$arr['building_costs_food'];
						$p = ($r*(1-pow($arr['building_build_costs_factor'],$level))
						/(1-$arr['building_build_costs_factor'])) 
						/ $cfg->p1('points_update');
						
						dbquery("
						INSERT INTO 
							building_points
	          (
	          	bp_building_id,
	            bp_level,
	            bp_points
	          ) 
						VALUES 
	            (".$arr['building_id'].",
	            '".$level."',
	            '".$p."');");
					}
				}
			}
			if ($mnr>0)
				return "Die Geb&auml;udepunkte von $mnr Geb&auml;uden wurden aktualisiert!";
	}

	function calcTechPoints($id=0)
	{
		$cfg = Config::getInstance();
		if ($id>0)
			$sql = "SELECT
				tech_id,
	      tech_costs_metal,
	      tech_costs_crystal,
	      tech_costs_fuel,
	      tech_costs_plastic,
	      tech_costs_food,
				tech_build_costs_factor,
	      tech_last_level
			FROM
				technologies
			WHERE
				tech_id=".$id.";";
		else	
			$sql = "SELECT
				tech_id,
	      tech_costs_metal,
	      tech_costs_crystal,
	      tech_costs_fuel,
	      tech_costs_plastic,
	      tech_costs_food,
				tech_build_costs_factor,
	      tech_last_level
			FROM
				technologies;";
			dbquery("DELETE FROM tech_points;");
			$res = dbquery($sql);
			$mnr = mysql_num_rows($res);
			if ($mnr>0)
			{
				while ($arr = mysql_fetch_array($res))
				{
					for ($level=1;$level<=intval($arr['tech_last_level']);$level++)
					{
						$r = $arr['tech_costs_metal']
						+$arr['tech_costs_crystal']
						+$arr['tech_costs_fuel']
						+$arr['tech_costs_plastic']
						+$arr['tech_costs_food'];
						$p = ($r*(1-pow($arr['tech_build_costs_factor'],$level))
						/(1-$arr['tech_build_costs_factor'])) 
						/ $cfg->p1('points_update');
						
						dbquery("
						INSERT INTO 
							tech_points
	          (
	          	bp_tech_id,
	            bp_level,
	            bp_points
	          ) 
						VALUES 
	            (".$arr['tech_id'].",
	            '".$level."',
	            '".$p."');");
					}
				}
			}
			if ($mnr>0)
				return "Die Punkte von $mnr Technologien wurden aktualisiert!";
	}

	function calcShipPoints()
	{
		$cfg = Config::getInstance();
		$res = dbquery("
		SELECT
			ship_id,
              ship_costs_metal,
              ship_costs_crystal,
              ship_costs_fuel,
              ship_costs_plastic,
              ship_costs_food
		FROM
			ships;");
		$mnr = mysql_num_rows($res);
		if ($mnr>0)
		{
			while ($arr = mysql_fetch_array($res))
			{
				$p = ($arr['ship_costs_metal']
				+$arr['ship_costs_crystal']
				+$arr['ship_costs_fuel']
				+$arr['ship_costs_plastic']
				+$arr['ship_costs_food'])
				/$cfg->p1('points_update');
				dbquery("
				UPDATE
					ships
				SET
					ship_points=".$p."
				WHERE
					ship_id=".$arr['ship_id'].";");
			}
		}
		return "Die Punkte von $mnr Schiffen wurden aktualisiert!";		
	}
	
	function calcDefensePoints()
	{
		$cfg = Config::getInstance();		
			$res = dbquery("
			SELECT
				def_id,
        def_costs_metal,
        def_costs_crystal,
        def_costs_fuel,
        def_costs_plastic,
        def_costs_food
			FROM
				defense;");
			$mnr = mysql_num_rows($res);
			if ($mnr>0)
			{
				while ($arr = mysql_fetch_array($res))
				{
					$p = ($arr['def_costs_metal']+
					$arr['def_costs_crystal']
					+$arr['def_costs_fuel']
					+$arr['def_costs_plastic']
					+$arr['def_costs_food'])
					/$cfg->p1('points_update');
					dbquery("UPDATE 
					defense
					 SET 
						def_points=$p
					WHERE 
						def_id=".$arr['def_id'].";");
				}
			}
			if ($mnr>0)
				return "Die Battlepoints von $mnr Verteidigungsanlagen wurden aktualisiert!";			
		
	}

?>
