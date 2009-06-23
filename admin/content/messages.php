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
	// 	Dateiname: messages.php
	// 	Topic: Nachrichtenverwaltung
	// 	Autor: Nicolas Perrenoud alias MrCage
	// 	Erstellt: 01.12.2004
	// 	Bearbeitet von: Nicolas Perrenoud alias MrCage
	// 	Bearbeitet am: 31.03.2006
	// 	Kommentar:
	//

	define("USER_MESSAGE_CAT_ID",1);
	define("SYS_MESSAGE_CAT_ID",5);

	echo "<h1>Nachrichten</h1>";

	//
	// Send message form
	//
	if ($sub=="sendmsg")
	{
		echo "Nachricht an einen Spieler senden:<br/><br/>";

		$subj = isset($_GET['message_subject']) ? $_POST['message_subject'] : "";
		$text = "";
		if (isset($_POST['submit']))
		{
			if ($_POST['message_subject']!="" && $_POST['message_text']!="")
			{
				$to = array();
				if ($_POST['rcpt_type']==1)
				{
					// Not good style, should use class.. but is faster for this ammount of data
					$res = dbquery("SELECT user_id,user_nick,user_email FROM users");
					if (mysql_num_rows($res)>0)
					{
						while($arr=mysql_fetch_assoc($res))
						{
							$to[$arr['user_id']] = $arr['user_nick']."<".$arr['user_email'].">";
						}
					}
				}
				else
				{
					$tu = new User($_POST['message_user_to']);
					$to[$_POST['message_user_to']] = $tu->nick."<".$tu->email.">";
				}

				$msg_type = $_POST['msg_type'];

				if ($msg_type==1 || $msg_type==2)
				{
					$mail = new Mail($_POST['message_subject'],$_POST['message_text']);
					if ($_POST['from_id']>0)
					{
						$atu = new User($cu->playerId);
						$reply = $atu->nick."<".$atu->email.">";
					}
					else
					{
						$reply = "";
					}					
				}

				$mailCnt = 0;
				$msgCnt = 0;

				foreach ($to as $k=>$v)
				{
					if ($msg_type==0 || $msg_type==2)
					{
						Message::sendFromUserToUser($_POST['from_id'],$k,$_POST['message_subject'],$_POST['message_text']);
						$msgCnt++;
					}
					if ($msg_type==1 || $msg_type==2)
					{
						$mail->send($v,$reply);
						$mailCnt++;
					}
				}
				if ($msgCnt>0)
					ok_msg("$msgCnt InGame-Nachrichten wurden versendet!");
				if ($mailCnt>0)
					ok_msg("$mailCnt Mails wurden versendet!");

			}
			else
			{
				cms_err_msg("Nachricht konnte nicht gesendet werden! Text oder Titel fehlt!");
			}
			$subj = $_POST['message_subject'];
			$text = $_POST['message_text'];
		}

			echo "<form action=\"?page=$page&sub=$sub\" method=\"POST\">";
			echo "<table width=\"300\" class=\"tb\">";
			echo "<tr>
				<th width=\"50\">Sender:</th>
				<td>";
			$fres = dbquery("
			SELECT
				user_nick,
				user_id,
				user_email,
				user_name
			FROM
				users
			WHERE 
				user_id=".intval($cu->playerId)."
			");
	
			if (mysql_num_rows($fres)>0)
			{
				$farr = mysql_fetch_assoc($fres);
				echo "<input type=\"radio\" name=\"from_id\" value=\"".$cu->playerId."\" checked=\"checked\" /> ".$farr['user_nick']." (InGame-Account #".$farr['user_id'].")<br/>";
				echo "<input type=\"radio\" name=\"from_id\" value=\"0\" /> System<br/>";
			}
			else
				echo "System <input type=\"hidden\" name=\"from_id\" value=\"0\" />";
			echo "</td></tr>";
			echo "<tr>
				<th>Empf&auml;nger:</th>
				<td class=\"tbldata\" width=\"250\">
				<b>An:</b> <input type=\"radio\" name=\"rcpt_type\" value=\"0\"  checked=\"checked\"  onclick=\"document.getElementById('message_user_to').style.display='';\" /> Einzelner Empfänger
				<select name=\"message_user_to\" id=\"message_user_to\">";
				$res=dbquery("SELECT user_id,user_nick FROM users ORDER BY user_nick;");
				while ($arr=mysql_fetch_array($res))
				{
					echo "<option value=\"".$arr['user_id']."\"";
					echo ">".$arr['user_nick']."</option>";
				}
			echo "</select> &nbsp; 
			<input type=\"radio\" name=\"rcpt_type\" value=\"1\" onclick=\"document.getElementById('message_user_to').style.display='none';\" /> Alle Spieler
			<br/>
			<b>Typ:</b>
			<input type=\"radio\" name=\"msg_type\" value=\"0\"  checked=\"checked\" /> InGame-Nachricht
			<input type=\"radio\" name=\"msg_type\" value=\"1\" /> E-Mail
			<input type=\"radio\" name=\"msg_type\" value=\"2\" /> InGame-Nachricht &amp; E-Mail
			</td></tr>";
			echo "<tr>
				<th>Betreff:</th>
				<td><input type=\"text\" name=\"message_subject\" value=\"".$subj."\" size=\"60\" maxlength=\"255\"></td></tr>";
			echo "<tr>
				<th>Text:</th>
				<td><textarea name=\"message_text\" rows=\"10\" cols=\"60\">".$text."</textarea></td></tr>";
			echo "</table>";
			echo "<p align=\"center\"><input type=\"submit\" class=\"button\" name=\"submit\" value=\"Senden\"></p>";
			echo "</form>";
		
	}



/************************
* Nachrichtenverwaltung *
************************/
	else
	{
		//
		// Suchresultate
		//
		if ($_POST['user_search']!="" || $_GET['action']=="searchresults")
		{
			if ($_SESSION['admin']['message_query']=="")
			{
				if ($_POST['message_user_from_id']!="")
					$sql.= " AND message_user_from=".$_POST['message_user_from_id'];
				if ($_POST['message_user_from_nick']!="")
				{
					$uid = get_user_id($_POST['message_user_from_nick']);
					if ($uid>0)
						$sql.= " AND message_user_from=$uid";
				}
				if ($_POST['message_user_to_id']!="")
					$sql.= " AND message_user_to=".$_POST['message_user_to_id'];
				if ($_POST['message_user_to_nick']!="")
				{
					$uid = get_user_id($_POST['message_user_to_nick']);
					if ($uid>0)
						$sql.= " AND message_user_to=$uid";
				}
				if ($_POST['message_subject']!="")
				{
					if (stristr($_POST['qmode']['message_subject'],"%")) $addchars = "%";else $addchars = "";
					$sql.= " AND md.subject ".stripslashes($_POST['qmode']['message_subject']).$_POST['message_subject']."$addchars'";
				}
				if ($_POST['message_text']!="")
				{
					if (stristr($_POST['qmode']['message_text'],"%")) $addchars = "%";else $addchars = "";
					$sql.= " AND md.text ".stripslashes($_POST['qmode']['message_text']).$_POST['message_text']."$addchars'";
				}
				if ($_POST['message_fleet_id']!="")
					$sql.= " AND md.fleet_id=".$_POST['message_fleet_id'];
				if ($_POST['message_entity_id']!="")
					$sql.= " AND md.entity_id=".$_POST['message_entity_id'];
				if ($_POST['message_read']<2)
				{
					if ($_POST['message_read']==1)
						$sql.= " AND (message_read=1)";
					else
						$sql.= " AND (message_read=0)";
				}
				if ($_POST['message_massmail']<2)
				{
					if ($_POST['message_massmail']==1)
						$sql.= " AND (message_massmail=1)";
					else
						$sql.= " AND (message_massmail=0)";
				}
				if ($_POST['message_deleted']<2)
				{
					if ($_POST['message_deleted']==1)
						$sql.= " AND (message_deleted=1)";
					else
						$sql.= " AND (message_deleted=0)";
				}
				if ($_POST['message_cat_id']!="")
					$sql.= " AND message_cat_id=".$_POST['message_cat_id'];

				if ($_POST['message_limit']!="")
					$limit=" LIMIT ".$_POST['message_limit'].";";
				else
					$limit=";";

				$sqlstart = "SELECT 
					message_id,
					message_user_from,
					message_user_to,
					md.subject,
					md.text,
					message_timestamp,
					message_deleted,
					message_read,
					message_archived,
					cat_name 
					FROM 
						messages 
					INNER JOIN
						 message_data as md 
						 ON message_id=md.id
					INNER JOIN
						message_cat
						ON message_cat_id=cat_id 
					WHERE 1 ";
				$sqlend = " ORDER BY message_timestamp DESC";
				$sql = $sqlstart.$sql.$sqlend.$limit;
				$_SESSION['admin']['message_query']=$sql;
			}
			else
				$sql = $_SESSION['admin']['message_query'];

			$res = dbquery($sql);
			if (mysql_num_rows($res)>0)
			{
				echo mysql_num_rows($res)." Datens&auml;tze vorhanden<br/><br/>";
				if (mysql_num_rows($res)>20)
					echo "<input type=\"button\" onclick=\"document.location='?page=$page'\" value=\"Neue Suche\" /><br/><br/>";

				echo "<b>Legende:</b> <span style=\"color:#0f0;\">Ungelesen</span>, <span style=\"color:#f90;\">Gel&ouml;scht</span>, <span style=\"font-style:italic;\">Archiviert</span><br/><br/>";

				echo "<table class=\"tb\">";
				echo "<tr>";
				echo "<th>Sender</th>";
				echo "<th>Empf&auml;nger</th>";
				echo "<th>Betreff</th>";
				echo "<th>Datum</th>";
				echo "<th>Kategorie</th>";
				echo "<th>Aktion</th>";
				echo "</tr>";
				while ($arr = mysql_fetch_array($res))
				{
					if ($arr['message_user_from']>0)
						$uidf = get_user_nick($arr['message_user_from']);
					else
						$uidf = "<i>System</i>";
					if ($arr['message_user_to']>0)
						$uidt = get_user_nick($arr['message_user_to']);
					else
						$uidt = "<i>System</i>";

					if ($arr['message_deleted']==1)
						$style="style=\"color:#f90\"";
					elseif ($arr['message_read']==0)
						$style="style=\"color:#0f0\"";
					elseif($arr['message_archived']==1)
						$style="style=\"font-style:italic;\"";
					else
						$style="";
					echo "<tr>";
					echo "<td $style>".cut_string($uidf,11)."</a></td>";
					echo "<td $style>".cut_string($uidt,11)."</a></td>";
					echo "<td $style ".mTT($arr['subject'],text2html(substr($arr['text'], 0, 500))).">".cut_string($arr['subject'],20)."</a></td>";
					echo "<td $style>".date("Y-d-m H:i",$arr['message_timestamp'])."</a></td>";
					echo "<td $style>".$arr['cat_name']."</td>";
					echo "<td>".edit_button("?page=$page&sub=edit&message_id=".$arr['message_id'])."</td>";
					echo "</tr>";
				}
				echo "</table><br/>";
				echo "<input type=\"button\" onclick=\"document.location='?page=$page'\" value=\"Neue Suche\" />";
			}
			else
			{
				echo "Die Suche lieferte keine Resultate!<br/><br/>";
				echo "<input type=\"button\" onclick=\"document.location='?page=$page'\" value=\"Zur&uuml;ck\" /><br/><br/>";
			}
		}

		elseif ($_GET['sub']=="edit")
		{
			$res = dbquery("
			SELECT 
				* 
			FROM 
				messages
			INNER JOIN
				message_data as md ON md.id=message_id AND  message_id=".$_GET['message_id'].";");
			$arr = mysql_fetch_array($res);
			if ($arr['message_user_from']>0)
				$uidf = get_user_nick($arr['message_user_from']);
			else
				$uidf = "<i>System</i>";
			if ($arr['message_user_to']>0)
				$uidt = get_user_nick($arr['message_user_to']);
			else
				$uidt = "<i>System</i>";
			echo "<table class=\"tbl\">";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">ID</td><td class=\"tbldata\">".$arr['message_id']."</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Sender</td><td class=\"tbldata\">$uidf</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Empf&auml;nger</td><td class=\"tbldata\">$uidt</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Datum</td><td class=\"tbldata\">".date("Y-m-d H:i:s",$arr['message_timestamp'])."</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Betreff</td><td class=\"tbldata\">".text2html($arr['subject'])."</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Text</td><td class=\"tbldata\">".text2html($arr['text'])."</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Quelltext</td>
			<td class=\"tbldata\"><textarea rows=\"20\" cols=\"80\" readonly=\"readonly\">".stripslashes($arr['text'])."</textarea></td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Gelesen?</td><td class=\"tbldata\">";
			switch ($arr['message_read'])
			{
				case 1: echo "Ja"; break;case 0: echo "Nein";break;
			}
			echo "</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Gel&ouml;scht?</td><td class=\"tbldata\">";
			switch ($arr['message_deleted'])
			{
				case 1: echo "Ja"; break;case 0: echo "Nein";break;
			}
			echo "</td></tr>";
			echo "<tr><td class=\"tbltitle\" valign=\"top\">Rundmail?</td><td class=\"tbldata\">";
			switch ($arr['message_massmail'])
			{
				case 1: echo "Ja"; break;case 0: echo "Nein";break;
			}
			echo "</td></tr>";

			echo "</table><br/><input type=\"button\" onclick=\"document.location='?page=$page&amp;action=searchresults'\" value=\"Zur&uuml;ck zu den Suchergebnissen\" /> &nbsp;
			<input type=\"button\" onclick=\"document.location='?page=$page'\" value=\"Neue Suche\" />";
		}

		else
		{
			$_SESSION['admin']['message_query']=null;
			echo "Suchmaske:<br/><br/>";
			echo "<form action=\"?page=$page\" method=\"post\">";
			echo "<table class=\"tb\">";
			echo "<tr><th style=\"width:130px;\">Sender-ID</th><td><input type=\"text\" name=\"message_user_from_id\" value=\"\" size=\"4\" maxlength=\"250\" /></td>";
			echo "<tr><th>Sender-Nick</th><td><input type=\"text\" name=\"message_user_from_nick\" id=\"message_user_from_nick\" value=\"\" size=\"20\" maxlength=\"250\" autocomplete=\"off\" onkeyup=\"xajax_searchUser(this.value,'message_user_from_nick','citybox');\" /><br><div class=\"citybox\" id=\"citybox\">&nbsp;</div></td>";
			echo "<tr><th>Empf&auml;nger-ID</th><td><input type=\"text\" name=\"message_user_to_id\" value=\"\" size=\"4\" maxlength=\"250\" /></td>";
			echo "<tr><th>Empf&auml;nger-Nick</th><td><input type=\"text\" name=\"message_user_to_nick\" id=\"message_user_to_nick\" value=\"\" size=\"20\" maxlength=\"250\" autocomplete=\"off\" onkeyup=\"xajax_searchUser(this.value,'message_user_to_nick','citybox1');\" /><br><div class=\"citybox\" id=\"citybox1\">&nbsp;</div></td>";
			echo "<tr><th>Betreff</th><td><input type=\"text\" name=\"message_subject\" value=\"\" size=\"20\" maxlength=\"250\" /> ";fieldqueryselbox('message_subject');echo "</td></tr>";
			echo "<tr><th>Text</th><td><input type=\"text\" name=\"message_text\" value=\"\" size=\"20\" maxlength=\"250\" /> ";fieldqueryselbox('message_text');echo "</td></tr>";
			echo "<tr><th style=\"width:130px;\">Flotten-ID</th><td><input type=\"text\" name=\"message_fleet_id\" value=\"\" size=\"4\" maxlength=\"250\" /></td>";
			echo "<tr><th style=\"width:130px;\">Entitiy-ID</th><td><input type=\"text\" name=\"message_entity_id\" value=\"\" size=\"4\" maxlength=\"250\" /></td>";
			echo "<tr><th>Gelesen</th><td><input type=\"radio\" name=\"message_read\" value=\"2\" checked=\"checked\" /> Egal
			<input type=\"radio\" name=\"message_read\" value=\"0\" /> Nein
			<input type=\"radio\" name=\"message_read\" value=\"1\" /> Ja</td></tr>";
			echo "<tr><th>Rundmail</th><td><input type=\"radio\" name=\"message_massmail\" value=\"2\" checked=\"checked\" /> Egal
			<input type=\"radio\" name=\"message_massmail\" value=\"0\" /> Nein
			<input type=\"radio\" name=\"message_massmail\" value=\"1\" /> Ja</td></tr>";
			echo "<tr><th>Gel&ouml;scht</th><td><input type=\"radio\" name=\"message_deleted\" value=\"2\" checked=\"checked\" /> Egal
			<input type=\"radio\" name=\"message_deleted\" value=\"0\" /> Nein
			<input type=\"radio\" name=\"message_deleted\" value=\"1\" /> Ja</td></tr>";
			echo "<tr><th>Kategorie</th><td><select name=\"message_cat_id\">";
			echo "<option value=\"\">(egal)</option>";
			$cres = dbquery("SELECT cat_id,cat_name FROM message_cat ORDER BY cat_order;");
			while ($carr = mysql_fetch_array($cres))
			{
				echo "<option value=\"".$carr['cat_id']."\">".$carr['cat_name']."</option>";
			}
			echo "</select></tr>";
			echo "<tr><th>Anzahl Datens&auml;tze</th><td class=\"tbldata\"><select name=\"message_limit\">";
			for ($x=100;$x<=2000;$x+=100)
				echo "<option value=\"$x\">$x</option>";
			echo "</select></td></tr>";

			echo "</table>";
			echo "<br/><input type=\"submit\" class=\"button\" name=\"user_search\" value=\"Suche starten\" /></form>";

			$tblcnt = mysql_fetch_row(dbquery("SELECT count(*) FROM messages;"));
			echo "<br/>Es sind ".nf($tblcnt[0])." Eintr&auml;ge in der Datenbank vorhanden.";
		}

	}
?>

