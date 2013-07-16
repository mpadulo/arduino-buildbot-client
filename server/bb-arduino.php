<?php
	$errors = false;
	$builders = json_decode(join('', file('http://localhost:8010/json/builders?as_text=1')));
	foreach ($builders as $buildername => $builder) {
		$detail = json_decode(join('', file('http://localhost:8010/json/builders/' . $buildername . '/builds/-1?as_text=1')));
		if (count($detail->{'times'}) == 2 )
			if ($detail->{'times'}[1] != null) {
				if (count($detail->{'text'}) > 1) {
					if ($detail->{'text'}[1] != 'successful')
						$errors = true;
				} else {
					$errors = true;
				}
			}
		}
	}

	if ($errors)
		echo '1';
	else
		echo '0';
?>
