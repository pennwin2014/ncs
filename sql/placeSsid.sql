drop table if exists nctermssid;
CREATE TABLE `nctermssid` (
  `ssid` char(64) COLLATE latin1_bin NOT NULL,
  `servicecode` char(14) COLLATE latin1_bin NOT NULL,
  `starttime` bigint(20) NOT NULL,
  `moditime` bigint(20) NOT NULL,
  UNIQUE KEY `nctermssid_ind1` (`ssid`,`servicecode`),
  KEY `nctermssid_ind2` (`moditime`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_bin MAX_ROWS=1000000000 AVG_ROW_LENGTH=1500;