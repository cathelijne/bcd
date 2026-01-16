<?php

function abGetAllFields($pro_num){
	$tmpArr = get_field_objects($pro_num);
	echo '<pre>'; print_r($tmpArr); echo '<pre>';
	$fillArray = array();
	foreach( $tmpArr as $tmpFieldObject ) {
		$fillArray = abGetAllFieldsCycle($fillArray, $tmpFieldObject["name"], $tmpFieldObject["value"]);
	}
	return $fillArray;
}

function abGetAllFieldsCycle($fillArray, $name, $value) {
	if (is_array($value)) {
		foreach( $value as $key => $value ) {
			$fillArray = abGetAllFieldsCycle($fillArray, $key, $value);
		}
	} else {
		array_push($fillArray, [$name, $value]);
	}
	return $fillArray;
}

$myFieldArray = abGetAllFields($user_id);


// $groups = acf_get_field_groups(array('post_id' => $post_id));
// if ($groups) {
//   foreach ($groups as $index => $group) {
//     $groups[$index]['fields'] = acf_get_fields($group['key']);
//   }
// }
// 
echo '<pre>'; print_r($myFieldArray); echo '<pre>';